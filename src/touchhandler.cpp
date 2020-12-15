#include <Arduino.h>
#define FlashFS LittleFS

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "touchhandler.h"

#define DISPLAY_WIDTH tft.width()
#define DISPLAY_HEIGHT tft.height()


const char *fourOptionsTexts[] = {"Greeting", "Message", "Update", "Nothing"};


TFT_eSPI *TouchHandler::_tft;
Scheduler *TouchHandler::_scheduler;
Task *TouchHandler::_task;
boolean TouchHandler::_buttonMode;
ButtonCallback TouchHandler::_buttonCallback;

TouchHandler::TouchHandler(Scheduler *scheduler, TFT_eSPI *tft) {
    _scheduler = scheduler;
    _tft = tft;    

}

void TouchHandler::setButtonCallback(ButtonCallback callback) {
    _buttonCallback = callback;
}

void TouchHandler::begin(uint16_t calData[]) {
    (*_tft).setTouch(calData);
    
    _task = new Task(13, TASK_FOREVER, _taskMethod, _scheduler, true, NULL, NULL);
}

void TouchHandler::resume() {
    _task->enable();
}


void TouchHandler::showButtons(ButtonCallback callback) {
    setButtonCallback(callback);
    showButtons();
}

void TouchHandler::showButtons() {
    Serial.println(F("Starting DisplayButtons"));
    (*_tft).fillScreen(TFT_BLACK);

    _buttonMode = true;
    (*_tft).fillScreen(TFT_BLACK);
    (*_tft).setTextDatum(CC_DATUM);
    (*_tft).setTextFont(2);
    (*_tft).drawString("Make a choice", (*_tft).width() / 2, 10);
    (*_tft).setTextFont(4);

    uint16_t paddingTop = 20;
    uint16_t padding = 10;
    uint16_t w = ((*_tft).width() - 4 * padding) / 2;
    uint16_t h = ((*_tft).height() - paddingTop - 4 * padding) / 2;
    Serial.printf("paddingTop: %d, padding: %d, w: %d, h: %d\n", paddingTop, padding, w, h);
    for (uint16_t row = 0; row < 2; row++) {
        for (uint16_t col = 0; col < 2; col++) {
            uint16_t x = padding + col * (w + 2 * padding);
            uint16_t y = paddingTop + padding + row * (h + 2 * padding);
            Serial.printf("x: %d, y: %d\n", x, y);
            (*_tft).drawRoundRect(x, y, w, h, 5, TFT_GREEN);
            (*_tft).drawString(fourOptionsTexts[row * 2 + col], x + w / 2, y + h / 2);
        }
    }
}

void TouchHandler::end() {
    _task->disable();

    // TFT_eSPI tft = *_tft;
}

void TouchHandler::_taskMethod() {

    uint16_t x, y;
    if ((*_tft).getTouch(&x, &y, 20))
    {
        Serial.printf("Got a touch event with x,y = %d, %d\n", x, y);
        (*_scheduler).disableAll();
        // Start touch after delay to prevent double touch events
        _task->enableDelayed(200);
        // tft.drawCircle(x, y, 5, TFT_RED);
        if (_buttonMode)
        {
            Serial.println(F("Buttonmode is true"));
            uint16_t paddingTop = 20;
            uint16_t mx = (*_tft).width() / 2;
            uint16_t my = (paddingTop + ((*_tft).height() - paddingTop) / 2);
            uint16_t index = (y > my) * 2 + (x > mx);
            // Serial.printf("paddingTop: %d, padding: %d, mx: %d, my: %d, index: %d\n", paddingTop, padding, mx, my, index);
            _buttonMode = false;
            Serial.println(F("Calling button callback"));
            _buttonCallback(index);
        }
        else
        {
            Serial.println(F("Buttonmode is false"));
            showButtons();
        }
    }
}
