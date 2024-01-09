// utils.h
#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"
#include "weathertypes.h"

WeatherInfo parseJsonInfo(String payload);

#endif