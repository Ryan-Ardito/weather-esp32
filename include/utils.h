// utils.h
#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"
#include "weathertypes.h"

String getStateFullName(const String& stateCode);
String expandCityStateString(const String& cityState);
WeatherInfo parseJsonInfo(String payload);

#endif