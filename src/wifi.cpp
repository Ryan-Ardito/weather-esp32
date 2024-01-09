#include <WiFiManager.h>

#include "consts.h"
#include "persist.h"
#include "display.h"

// Forget wifi on startup, showing config page every time.
bool dev_reset = true;

bool force_config;
bool should_save_config;

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
  if (dev_reset)
  {
    wm.resetSettings();
  }
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