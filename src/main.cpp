// TODO: Save more location info in a JSON file.

#include "heltec.h"
#include "Arduino.h"
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <FS.h>

#include "consts.h"
#include "utils.h"
#include "weathertypes.h"
#include "persist.h"
#include "owm_api.h"
#include "display.h"

bool force_config;
bool should_save_config;

Coords coords;

void LedON(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void VextON(void)
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) // Vext default OFF
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

void delayBlink(int duration_millis)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(duration_millis / 2);
  digitalWrite(LED_BUILTIN, LOW);
  delay(duration_millis / 2);
}

void saveConfigCallback(void)
{
  should_save_config = true;
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  showSetupInstructions();
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setupWifiConfig(void)
{
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.resetSettings();
  // wm.setDebugOutput(false);
  wm.setWiFiAutoReconnect(true);
  wm.setConfigPortalTimeout(180);

  const char *location = readConfig().c_str();
  // id/name, placeholder/prompt, default, length
  WiFiManagerParameter weather_location("location", "city, state", location, 40);

  wm.addParameter(&weather_location);

  std::vector<const char *> menu = {"wifi"};
  wm.setMenu(menu);
  wm.setTitle("Config");
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setAPCallback(configModeCallback);

  if (force_config)
  {
    if (!wm.startConfigPortal(ap_name, ap_password))
    {
      Serial.println("Connection failed!");
      // ESP.restart();
    }
  }
  else
  {
    if (!wm.autoConnect(ap_name, ap_password))
    {
      Serial.println("Connection failed!");
      // ESP.restart();
    }
  }

  auto new_location = weather_location.getValue();
  if (should_save_config && new_location != location)
  {
    writeFile(LittleFS, "/data.txt", new_location);
  }
}


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