#include "Arduino.h"
#include <ArduinoJson.h>
#include "weathertypes.h"

String unixTimeToString(int unix_time, int zone_offset)
{
  int zone_time = unix_time + zone_offset;
  int hrs = (zone_time / 3600) % 12;
  if (hrs == 0)
  {
    hrs = 12;
  }
  String hrs_string = String(hrs);
  if (hrs < 10)
  {
    hrs_string = String(" " + hrs_string);
  }

  int mins = (zone_time / 60) % 60;
  String mins_string = String(mins);
  if (mins < 10)
  {
    mins_string = String("0" + mins_string);
  }

  return String(hrs_string + ":" + mins_string);
}

WeatherInfo parseJsonInfo(String payload)
{
  WeatherInfo info;

  JsonDocument doc;
  const auto err = deserializeJson(doc, payload);

  const auto main = doc["main"];
  const auto datetime = (const uint32_t)doc["dt"];
  const auto weather = doc["weather"][0];
  const auto timezone_offset = (const int32_t)doc["timezone"];
  info.clock_time = unixTimeToString(datetime, timezone_offset);
  info.location = (const char *)doc["name"];
  info.description = (const char *)weather["description"];
  info.temperature = (const float)main["temp"];
  info.humidity = (const uint8_t)main["humidity"];

  return info;
}