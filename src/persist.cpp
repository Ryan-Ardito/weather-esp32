#include "Arduino.h"
#include <LittleFS.h>
#include <FS.h>

#include "owm_api.h"

String readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while (file.available())
  {
    fileContent += String((char)file.read());
  }
  file.close();
  return fileContent;
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- write failed");
  }
  file.close();
}

String readLocation(void)
{
  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS mount failed");
    return "";
  }
  else
  {
    Serial.println("LittleFS mount success");
  }

  String location = readFile(LittleFS, "/location.txt");
  return location;
}

Coords readCoords(void)
{
  Coords coords;

  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS mount failed");
    return coords;
  }
  else
  {
    Serial.println("LittleFS mount success");
  }

  String coords_string = readFile(LittleFS, "/coords.txt");

  // Find the position of the comma
  int commaPos = coords_string.indexOf(',');

  // Extract city and state substrings
  coords.lat = coords_string.substring(0, commaPos);
  coords.lon = coords_string.substring(commaPos + 1);

  return coords;
}