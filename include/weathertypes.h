// weathertypes.h
#ifndef WEATHERTYPES_H
#define WEATHERTYPES_H

// #include "heltec.h"
#include "Arduino.h"
// #include <WiFiManager.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include <LittleFS.h>
// #include <FS.h>

struct Coords
{
  String lat;
  String lon;
};

struct WeatherInfo
{
  String location;
  String description;
  String clock_time;
  float temperature;
  int humidity;
};

#endif