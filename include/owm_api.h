// owm_api.h
#ifndef OWM_API_H
#define OWM_API_H

// #include "heltec.h"
#include "Arduino.h"
// #include <WiFiManager.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include <LittleFS.h>
// #include <FS.h>
#include "weathertypes.h"

// String apiGet(String url);
Coords fetchLatLon(String city_state);
String fetchWeatherInfo(Coords coords);

#endif
