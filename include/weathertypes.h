// weathertypes.h
#ifndef WEATHERTYPES_H
#define WEATHERTYPES_H

#include "Arduino.h"

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