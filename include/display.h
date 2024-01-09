// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include "weathertypes.h"

void SetupDisplay(void);
void showSetupInstructions(void);
void displayWeather(WeatherInfo info);

#endif