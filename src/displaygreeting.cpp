#include <Arduino.h>
#define FlashFS LittleFS

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "displaygreeting.h"
#include <AnimatedGIF.h>
#include "mc-white.h"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define BUFFER_SIZE 320 // Optimum is >= GIF width or integral division of width
#define tft (*_tft)

uint16_t usTemp[1][BUFFER_SIZE]; // Global to support DMA use
bool dmaBuf = 0;
uint16_t test = 3444;

AnimatedGIF _animGif;

TFT_eSPI *DisplayGreeting::_tft;
Scheduler *DisplayGreeting::_scheduler;
Task *DisplayGreeting::_task;
boolean DisplayGreeting::_showGifInProgress;

DisplayGreeting::DisplayGreeting(Scheduler *scheduler, TFT_eSPI *aTft)
{
    DisplayGreeting::_scheduler = scheduler;
    DisplayGreeting::_tft = aTft;
}

void DisplayGreeting::begin()
{
    // Serial.printf("Test value: %u", test);
    Serial.println(F("Starting DisplayGreeting"));
    // TFT_eSPI tft = *_tft;
    tft.fillScreen(TFT_BLACK);

    _animGif.begin(BIG_ENDIAN_PIXELS);
    _showGifInProgress = false;
    _task = new Task(111, TASK_FOREVER, _taskMethod, _scheduler, true, NULL, _onDisable);
}

void DisplayGreeting::end()
{
    // TFT_eSPI tft = *_tft;
    Serial.println(F("end() called for display greeting"));
    _task->disable();
    if (_showGifInProgress)
    {
        _animGif.close();
        tft.endWrite();
        _showGifInProgress = false;
    }
}

void DisplayGreeting::_onDisable()
{
    Serial.println(F("onDisable() called for display greeting"));
    end();
}

void DisplayGreeting::_taskMethod()
{
    // TFT_eSPI tft = *_tft;
    if (_showGifInProgress)
    {
        if (!_animGif.playFrame(false, NULL))
        {
            _animGif.close();
            tft.endWrite();
            _showGifInProgress = false;
        }
    }
    else
    {
        _animGif.open((uint8_t *)mc_gif, sizeof(mc_gif), _drawGif);
        // Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", _animGif.getCanvasWidth(), _animGif.getCanvasHeight());
        tft.startWrite();
        _showGifInProgress = true;
    }
}

// Draw a line of image directly on the LCD
void DisplayGreeting::_drawGif(GIFDRAW *pDraw)
{
    // TFT_eSPI tft = *_tft;

    uint8_t *s;
    uint16_t *d, *usPalette;
    int x, y, iWidth, iCount;

    // Displ;ay bounds chech and cropping
    iWidth = pDraw->iWidth;
    if (iWidth + pDraw->iX > DISPLAY_WIDTH)
        iWidth = DISPLAY_WIDTH - pDraw->iX;
    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line
    if (y >= DISPLAY_HEIGHT || pDraw->iX >= DISPLAY_WIDTH || iWidth < 1)
        return;

    // Old image disposal
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
        for (x = 0; x < iWidth; x++)
        {
            if (s[x] == pDraw->ucTransparent)
                s[x] = pDraw->ucBackground;
        }
        pDraw->ucHasTransparency = 0;
    }

    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
        uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
        pEnd = s + iWidth;
        x = 0;
        iCount = 0; // count non-transparent pixels
        while (x < iWidth)
        {
            c = ucTransparent - 1;
            d = &usTemp[0][0];
            while (c != ucTransparent && s < pEnd && iCount < BUFFER_SIZE)
            {
                c = *s++;
                if (c == ucTransparent) // done, stop
                {
                    s--; // back up to treat it like transparent
                }
                else // opaque
                {
                    *d++ = usPalette[c];
                    iCount++;
                }
            }           // while looking for opaque pixels
            if (iCount) // any opaque pixels?
            {
                // DMA would degrtade performance here due to short line segments
                tft.setAddrWindow(pDraw->iX + x, y, iCount, 1);
                tft.pushPixels(usTemp, iCount);
                x += iCount;
                iCount = 0;
            }
            // no, look for a run of transparent pixels
            c = ucTransparent;
            while (c == ucTransparent && s < pEnd)
            {
                c = *s++;
                if (c == ucTransparent)
                    x++;
                else
                    s--;
            }
        }
    }
    else
    {
        s = pDraw->pPixels;

        // Unroll the first pass to boost DMA performance
        // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
        if (iWidth <= BUFFER_SIZE)
            for (iCount = 0; iCount < iWidth; iCount++)
                usTemp[dmaBuf][iCount] = usPalette[*s++];
        else
            for (iCount = 0; iCount < BUFFER_SIZE; iCount++)
                usTemp[dmaBuf][iCount] = usPalette[*s++];

#ifdef USE_DMA // 71.6 fps (ST7796 84.5 fps)
        tft.dmaWait();
        tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
        tft.pushPixelsDMA(&usTemp[dmaBuf][0], iCount);
        dmaBuf = !dmaBuf;
#else // 57.0 fps
        tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
        tft.pushPixels(&usTemp[0][0], iCount);
#endif

        iWidth -= iCount;
        // Loop if pixel buffer smaller than width
        while (iWidth > 0)
        {
            // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
            if (iWidth <= BUFFER_SIZE)
                for (iCount = 0; iCount < iWidth; iCount++)
                    usTemp[dmaBuf][iCount] = usPalette[*s++];
            else
                for (iCount = 0; iCount < BUFFER_SIZE; iCount++)
                    usTemp[dmaBuf][iCount] = usPalette[*s++];

#ifdef USE_DMA
            tft.dmaWait();
            tft.pushPixelsDMA(&usTemp[dmaBuf][0], iCount);
            dmaBuf = !dmaBuf;
#else
            tft.pushPixels(&usTemp[0][0], iCount);
#endif
            iWidth -= iCount;
        }
    }
} /* GIFDraw() */
