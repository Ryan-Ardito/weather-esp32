// #include "heltec.h"
#include "Arduino.h"
// #include <WiFiManager.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include <LittleFS.h>
// #include <FS.h>
#include "consts.h"

const int kSerialBaudRate = 115200;
const int kRequestIntervalMillis = 10 * 60 * 1000;
const char *ap_name = "Weather Daddy";
const char *ap_password = "password";
const String owm_base_url = "https://api.openweathermap.org/data/2.5/weather";
const String apikey = "9301cc18f16f719750a35990eb761e9e";
// const String lat = "38";
// const String lon = "-106";