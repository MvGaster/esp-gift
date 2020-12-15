#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <TFT_eSPI.h>

#include "displaymessage.h"


TFT_eSPI *DisplayMessage::_tft;
TFT_eSprite *DisplayMessage::_sprite;
Scheduler *DisplayMessage::_scheduler;
Task *DisplayMessage::_task;
String *DisplayMessage::_personalText;
int16_t DisplayMessage::_textWidth;
const char *DisplayMessage::_scrollText;
int in = 0;


DisplayMessage::DisplayMessage(Scheduler *scheduler, TFT_eSPI *tft, TFT_eSprite *sprite, String *aPersonalText)
{
  _tft = tft;
  _sprite = sprite;
  _scheduler = scheduler;
  _personalText = aPersonalText;
}

void DisplayMessage::begin() {
    Serial.println(F("Start showScrollingText"));
    _scrollText = (*_personalText).c_str();
    Serial.print(F("Showing text: "));
    Serial.println(_scrollText);
    (*_tft).fillScreen(TFT_BLACK);
    int index = 0;
    for (int y=0; y < ((*_tft).height() - 20) / 2; y = y + 10) {
        (*_tft).drawFastHLine(index * 2, y, (*_tft).width() - index * 4, TFT_BLUE);
        (*_tft).drawFastHLine(index * 2, (*_tft).height() - y, (*_tft).width() - index * 4, TFT_BLUE);
        index++;
    }


    (*_tft).setBitmapColor(TFT_GREEN, TFT_BLACK);

    (*_sprite).setColorDepth(1);
    (*_sprite).createSprite((*_tft).width(), 26);
    (*_sprite).fillSprite(TFT_BLACK);
    (*_sprite).setTextFont(4);
    (*_sprite).setTextColor(TFT_GREEN, TFT_BLACK);
    (*_sprite).setTextSize(1);
    _textWidth = (*_sprite).textWidth(_scrollText);
    in = 0;
    _task = new Task(10, TASK_FOREVER, _taskMethod, _scheduler, true, NULL, NULL);
}

void DisplayMessage::_taskMethod() {

    if (in > (_textWidth + (*_tft).width()))
    {
        in = 0;
        (*_sprite).fillSprite(TFT_BLACK);
    }
    (*_sprite).fillSprite(TFT_BLACK);
    (*_sprite).drawString(_scrollText, (*_tft).width() - in, 0);
    (*_sprite).pushSprite(0, (*_tft).height()/2 - 10);
    // (*_sprite).pushsprite(0, 0);
    in = in + 3;
}