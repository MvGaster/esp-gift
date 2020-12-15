#include <Arduino.h>

#include <FS.h>
#include <LittleFS.h>
#include "defines.h"
#include "filesystem.h"

String getPersonalTextFromFS()
{
  String result = "";
  Serial.println(F("Initializing FS..."));
  if (LittleFS.begin())
  {
    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail."));
  }
  Dir dir = LittleFS.openDir("/");

  while (dir.next())
  {
    Serial.print(F("Found file: "));
    Serial.print(dir.fileName());
    Serial.print(F(", with file size "));
    Serial.println(dir.fileSize());
    String filename = TEXT_FILE_NAME;
    Serial.print(F("Comparing filename against: "));
    Serial.println(filename);
    if (filename.equals(dir.fileName()))
    {

      File file = dir.openFile("r");
      result = file.readString();
      file.close();
      Serial.print(F("Content of file: "));
      Serial.println(result);
    }
  }
  LittleFS.end();
  return result;
}

void updatePersonalTextInFS(String text)
{
  if (LittleFS.begin())
  {
    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail."));
    return;
  }

  String filename = TEXT_FILE_NAME;
  File file = LittleFS.open("/" + filename, "w+");
  file.print(text);
  file.close();

  LittleFS.end();
  Serial.println(F("File updated"));
}
