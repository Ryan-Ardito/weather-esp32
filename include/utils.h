// utils.h
#ifndef UTILS_H
#define UTILS_H

// #include "heltec.h"
#include "Arduino.h"
// #include <WiFiManager.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include <LittleFS.h>
// #include <FS.h>
#include "weathertypes.h"

WeatherInfo parseJsonInfo(String payload);

#endif