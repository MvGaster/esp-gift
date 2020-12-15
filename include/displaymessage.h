#ifndef __DISPLAYMESSAGE_H__
#define __DISPLAYMESSAGE_H__


#include <TaskSchedulerDeclarations.h>
#include <TFT_eSPI.h>


class DisplayMessage
{
  public:
    DisplayMessage(Scheduler* scheduler, TFT_eSPI *tft, TFT_eSprite *sprite, String *aPersonalText);
    void begin();
  private:
    static Scheduler *_scheduler;
    static Task *_task;
    static TFT_eSPI *_tft;
    static TFT_eSprite *_sprite;
    static String *_personalText;
    static int16_t _textWidth;
    static const char *_scrollText;
    static void _taskMethod();
};
#endif // __DISPLAYMESSAGE_H__