#ifndef __DISPLAYCLOCK_H__
#define __DISPLAYCLOCK_H__

#include <TaskSchedulerDeclarations.h>
#include <TFT_eSPI.h>


class DisplayClock
{
  public:
    DisplayClock(Scheduler* scheduler, TFT_eSPI *tft);
    void begin();
  private:
    static Scheduler *_scheduler;
    static Task *_task;
    static TFT_eSPI *_tft;
    static void _taskMethod();
};

#endif // __DISPLAYCLOCK_H__