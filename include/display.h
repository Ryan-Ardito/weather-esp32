// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

// #include "heltec.h"
#include "Arduino.h"
// #include <WiFiManager.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include <LittleFS.h>
// #include <FS.h>
#include "weathertypes.h"

void SetupDisplay(void);
void showSetupInstructions(void);
void displayWeather(WeatherInfo info);

#endif