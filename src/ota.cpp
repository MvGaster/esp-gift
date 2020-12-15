#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <ArduinoOTA.h>
#include <TFT_eSPI.h>

#include "ota.h"

static const char* otaPassword = "lkjasdfsdf";

TFT_eSPI *OtaReady::_tft;
Scheduler *OtaReady::_scheduler;
Task *OtaReady::_task;




OtaReady::OtaReady(Scheduler *scheduler, TFT_eSPI *tft)
{
  OtaReady::_tft = tft;
  OtaReady::_scheduler = scheduler;

  ArduinoOTA.setPassword(otaPassword);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
    (*_tft).fillScreen(TFT_BLACK);
    (*_tft).setTextFont(2);
    (*_tft).setTextDatum(MC_DATUM);
    (*_tft).drawString("Starting ota update... Please wait for reboot.", (*_tft).width()/2, 100);


    
  });
  ArduinoOTA.onEnd([]() {
    Serial.println(F("\nEnd"));
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    (*_tft).setCursor(50,150);
    (*_tft).printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println(F("Auth Failed"));
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println(F("Begin Failed"));
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println(F("Connect Failed"));
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println(F("Receive Failed"));
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println(F("End Failed"));
    }
  });
}

boolean OtaReady::begin() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Not connected to wifi. Cannot do OTA update"));
    (*_tft).fillScreen(TFT_BLACK);
    (*_tft).setTextFont(2);
    (*_tft).drawString("Not connected to wifi. Cannot do OTA update.", 0, 100);
    delay(5000);
    return false;
  } else {
    (*_tft).fillScreen(TFT_BLACK);
    (*_tft).setTextFont(2);
    (*_tft).setTextDatum(MC_DATUM);
    (*_tft).drawString("Waiting for ota update...", (*_tft).width()/2, 100);
    (*_tft).drawString("(reset device if you don't have an ota update)", (*_tft).width()/2, 200);
    ArduinoOTA.begin();
    OtaReady::_task = new Task(5, TASK_FOREVER, _taskMethod, _scheduler, true, NULL, NULL);
    return true;
  }
}

void OtaReady::_taskMethod() {
  ArduinoOTA.handle();
}