// TODO: Invalid location string should notify the user and restart the config AP.
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

// Latitude and longitude to send to weather API
Coords coords;

void setup()
{
  Serial.begin(kSerialBaudRate);

  LedON();
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

  digitalWrite(LED_BUILTIN, HIGH);
  String info_json = fetchWeatherInfo(coords);
  WeatherInfo info = parseJsonInfo(info_json);
  digitalWrite(LED_BUILTIN, LOW);
  displayWeather(info);

  delay(kRequestIntervalMillis);
}