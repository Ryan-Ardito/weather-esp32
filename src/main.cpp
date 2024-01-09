// TODO: Save more location info in a JSON file.

#include "Arduino.h"
#include <WiFiManager.h>

#include "consts.h"
#include "utils.h"
#include "weathertypes.h"
#include "persist.h"
#include "owm_api.h"
#include "display.h"
#include "wifi.h"

Coords coords;

void setup()
{
  Serial.begin(kSerialBaudRate);

  LedON();
  VextON();
  delay(100);

  SetupDisplay();
  setupWifiConfig();

  String location = readConfig();
  coords = fetchLatLon(location);
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