#ifndef __TOUCHHANDLER_H__
#define __TOUCHHANDLER_H__



#include <TaskSchedulerDeclarations.h>
#include <TFT_eSPI.h>

typedef void (*ButtonCallback)(int);

class TouchHandler
{
  public:
    TouchHandler(Scheduler* scheduler, TFT_eSPI *tft);
    static void begin(uint16_t calData[]);
    static void setButtonCallback(ButtonCallback callback);
    static void showButtons(ButtonCallback callback);
    static void showButtons();
    static void end();
    static void resume();
  private:
    static Scheduler *_scheduler;
    static Task *_task;
    static TFT_eSPI *_tft;
    static void _taskMethod();
    static boolean _buttonMode;
    static ButtonCallback _buttonCallback;

};

#endif // __TOUCHHANDLER_H__