#ifndef __OTA_H__
#define __OTA_H__

#include <TaskSchedulerDeclarations.h>
#include <TFT_eSPI.h>


class OtaReady
{
  public:
    OtaReady(Scheduler* scheduler, TFT_eSPI *tft);
    boolean begin();
  private:
    static Scheduler *_scheduler;
    static Task *_task;
    static TFT_eSPI *_tft;
    static void _taskMethod();
};
#endif // __OTA_H__