#include <Arduino.h>
#include <TaskScheduler.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include "wifi.h"
#include "ota.h"
#include "filesystem.h"
#include "touchhandler.h"
#include "displaygreeting.h"
#include "displaymessage.h"

#include <FS.h>
#include <LittleFS.h>
#include "defines.h"

#define FlashFS LittleFS

#define INITIAL_STATE = 1

Scheduler runner;
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);


String personalText;
DeviceInit deviceInit(&runner, &personalText);
TouchHandler touchHandler(&runner, &tft);
DisplayGreeting displayGreeting(&runner, &tft);
DisplayMessage displayMessage(&runner, &tft, &sprite, &personalText);
OtaReady otaReady(&runner, &tft);

int displayBrightness = PWMRANGE;

void touch_calibrate();
void showGreeting();
void showMessage();
void otaUpdate();
void toBeImplemented();
void processButtonCallback(int index);



void setup()
{
  delay(100);
  Serial.begin(115200);
  Serial.println(F("\nStarting..."));
  tft.init();
  tft.setRotation(1);

  // displayGreeting.begin();

  personalText = getPersonalTextFromFS();

  // displaySetup();
#ifdef CALIBRATE_TOUCH
  touch_calibrate();
#endif

  uint16_t calData[5] = { 459, 3273, 509, 3150, 3 };
  // displaySetTouch(calData);
  touchHandler.begin(calData);
  deviceInit.begin();
  touchHandler.showButtons(processButtonCallback);

}


void processButtonCallback(int index)
{
  Serial.printf("processButtonEvent called with index %d\n", index);
  switch (index)
  {
  case 0:
    showGreeting();
    break;
  case 1:
    showMessage();
    break;
  case 2:
    otaUpdate();
    break;
  case 3:
    toBeImplemented();
    break;
  default:
    Serial.println(F("Received invalid button event index"));
    break;
  }
}

void showGreeting()
{
  Serial.println(F("Show greeting initiated"));
  displayGreeting.begin();
}

void showMessage()
{
  Serial.println(F("Show message initiated"));
  displayMessage.begin();
}

void otaUpdate()
{
  Serial.println(F("OTA update initiated"));
  runner.disableAll();
  if(!otaReady.begin()) {
    // couldn't start ota update
    touchHandler.showButtons(processButtonCallback);
    touchHandler.resume();
  }
}

void toBeImplemented()
{
    //nothing
}

void loop()
{
  runner.execute();
  yield();
}



// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate()
{
    uint16_t calData[5];

    // Calibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    Serial.println();
    Serial.println();
    Serial.println(F("// Use this calibration code in setup():"));
    Serial.print(F("  uint16_t calData[5] = "));
    Serial.print(F("{ "));

    for (uint8_t i = 0; i < 5; i++)
    {
        Serial.print(calData[i]);
        if (i < 4)
            Serial.print(F(", "));
    }

    Serial.println(F(" };"));
    Serial.print(F("  tft.setTouch(calData);"));
    Serial.println();
    Serial.println();

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");
    tft.println("Calibration code sent to Serial port.");

    delay(4000);
}