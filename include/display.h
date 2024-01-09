// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include "weathertypes.h"

void delayBlink(int duration_millis);
void LedON(void);
void VextON(void);
void VextOFF(void);
void SetupDisplay(void);
void showSetupInstructions(void);
void displayWeather(WeatherInfo info);

#endif