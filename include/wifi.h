#ifndef __WIFI_H__
#define __WIFI_H__

#include <TaskSchedulerDeclarations.h>

class DeviceInit
{
  public:
    DeviceInit(Scheduler* scheduler, String *aPersonalText);
    void begin();
  private:
    static Scheduler *_scheduler;
    static String *_personalText;
    static Task *_initTask;
    static void _initDevice();
};

#endif // __WIFI_H__