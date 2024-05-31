// TODO: User can start the config AP manually

#include "Arduino.h"
#include <WiFiManager.h>

#include "utils.h"
#include "weathertypes.h"
#include "persist.h"
#include "owm_api.h"
#include "display.h"
#include "wifi.h"

const int kSerialBaudRate = 115200;
const int kRequestIntervalMillis = 10 * 60 * 1000;

// Latitude and longitude sent to weather API
Coords coords;

void setup()
{
  LedON();
  Serial.begin(kSerialBaudRate);
  Serial.println("Booting...");
  VextON();
  delay(100);
  SetupDisplay();
  setupWifiConfig();

  coords = readCoords();
}

void loop()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    delayBlink(500);
  }

  LedON();
  String info_json = fetchWeatherInfo(coords);
  WeatherInfo info = parseJsonInfo(info_json);
  displayWeather(info);
  LedOFF();

  delay(kRequestIntervalMillis);
}