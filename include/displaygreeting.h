#ifndef __DISPLAYGREETING_H__
#define __DISPLAYGREETING_H__


#include <TaskSchedulerDeclarations.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>


class DisplayGreeting
{
  public:
    DisplayGreeting(Scheduler* scheduler, TFT_eSPI *tft);
    static void begin();
    static void end();
  private:
    static Scheduler *_scheduler;
    static Task *_task;
    static TFT_eSPI *_tft;
    // static AnimatedGIF _animGif;
    static void _taskMethod();
    static void _drawGif(GIFDRAW *pDraw);
    static void _onDisable();
    static boolean _showGifInProgress;
};

#endif // __DISPLAYGREETING_H__