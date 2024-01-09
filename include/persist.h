// persist.h
#ifndef PERSIST_H
#define PERSIST_H

// #include "heltec.h"
#include "Arduino.h"
// #include <WiFiManager.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
#include <LittleFS.h>
#include <FS.h>
// #include "weathertypes.h"

String readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
String readConfig(void);

#endif