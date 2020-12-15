#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <TFT_eSPI.h>

#include "displayclock.h"

TFT_eSPI *DisplayClock::_tft;
Scheduler *DisplayClock::_scheduler;
Task *DisplayClock::_task;

DisplayClock::DisplayClock(Scheduler *scheduler, TFT_eSPI *tft)
{
  _tft = tft;
  _scheduler = scheduler;
}

void DisplayClock::begin()
{
}

void DisplayClock::_taskMethod()
{
}