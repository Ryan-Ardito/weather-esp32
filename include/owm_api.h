// owm_api.h
#ifndef OWM_API_H
#define OWM_API_H

#include "Arduino.h"
#include "weathertypes.h"

Coords fetchLatLon(String city_state);
String fetchWeatherInfo(Coords coords);

#endif
