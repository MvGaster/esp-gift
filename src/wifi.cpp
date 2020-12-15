#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "wifi.h"
#include "filesystem.h"
#include "defines.h"

static const char *url = "http://10.0.0.113:3000/devices/";

Scheduler *DeviceInit::_scheduler;
String *DeviceInit::_personalText;
Task *DeviceInit::_initTask;

void DeviceInit::_initDevice()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(F("."));
  }
  else
  {
    Serial.println();
    Serial.print(F("Connected, IP address: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("Hostname: "));
    Serial.println(WiFi.hostname());

    HTTPClient http; //Object of class HTTPClient
    WiFiClient client;
    http.begin(client, url + WiFi.hostname());
    int httpCode = http.GET();

    if (httpCode == 200)
    {
      const size_t bufferSize = 52 + 256;
      DynamicJsonDocument doc(bufferSize);
      DeserializationError error = deserializeJson(doc, http.getString());
      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      const char *id = doc["id"];
      String text = String((const char *)doc["text"]);

      if (text.length() <= 1)
      {
        Serial.println(F("No valid text received from server"));
      }

      Serial.print(F("id:"));
      Serial.println(id);
      Serial.print(F("text:"));
      Serial.println(text);

      String storedText = (*DeviceInit::_personalText);

      if (storedText.length() > 0)
      {
        // update test of file if different
        if (!text.equals(storedText))
        {
          Serial.print(F("Personal text \""));
          Serial.print(storedText);
          Serial.println(F("\" is different than retrieved text"));

          updatePersonalTextInFS(text);
          *DeviceInit::_personalText = text;
        }
        else
        {
          Serial.print(F("Personal text \""));
          Serial.print(storedText);
          Serial.println(F("\" is the same as retrieved text"));
        }
      }
      else
      {
        updatePersonalTextInFS(text);
        // write text to file
      }
    }
    else
    {
      Serial.printf("Received statuscode %d on http request\n", httpCode);
    }
    Serial.println(F("Close http connection"));
    http.end(); //Close connection
    Serial.println(F("End task"));
    DeviceInit::_initTask->disable();
    Serial.println(F("End task done"));

  }
}

DeviceInit::DeviceInit(Scheduler *scheduler, String *aPersonalText)
{
  DeviceInit::_scheduler = scheduler;
  DeviceInit::_personalText = aPersonalText;
  Serial.print(F("Hostname: "));
  Serial.println(WiFi.hostname());
}

void DeviceInit::begin()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("esp_network", "kWHBfnk8oD7LJw");
  Serial.print(F("Connecting "));
  DeviceInit::_initTask = new Task(200, 50, _initDevice, _scheduler, true, NULL, NULL);
}

void deleteAllCredentials(void)
{
  Serial.println(F("Clearing all stored credentials"));

  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.begin();
  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {
    WiFi.disconnect();
    while (WiFi.status() == WL_CONNECTED)
      delay(100);
  }
  Serial.println(F("Credentials removed."));
}