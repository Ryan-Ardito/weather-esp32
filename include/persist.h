// persist.h
#ifndef PERSIST_H
#define PERSIST_H

#include "Arduino.h"
#include <LittleFS.h>
#include <FS.h>

String readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
String readConfig(void);

#endif