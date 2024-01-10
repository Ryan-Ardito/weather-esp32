#include <WiFiManager.h>

#include "persist.h"
#include "display.h"
#include "owm_api.h"
#include "utils.h"

const char *ap_name = "Weather Daddy";
const char *ap_password = "password";

// Forget wifi on startup, showing config page every time.
bool dev_mode = true;

bool force_config;
bool should_save_config;

void saveConfigCallback(void)
{
  showConnecting();
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
  if (dev_mode)
  {
    wm.resetSettings();
  }
  else
  {
    wm.setDebugOutput(false);
  }
  wm.setTitle("Config");
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setAPCallback(configModeCallback);
  wm.setWiFiAutoReconnect(true);
  // wm.setConfigPortalTimeout(180);

  // const char *location = readLocation().c_str(); doesn't work lol
  String loc = readLocation();
  const char *location = loc.c_str();

  // id/name, placeholder/prompt, default, length
  WiFiManagerParameter location_param("location", "city, state", location, 40);

  wm.addParameter(&location_param);

  std::vector<const char *> menu = {"wifi"};
  wm.setMenu(menu);

  showConnecting();
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

  auto new_location = location_param.getValue();
  if (should_save_config && new_location != location)
  {
    writeFile(LittleFS, "/location.txt", new_location);
    String expanded_location = expandCityStateString(new_location);
    Coords coords = fetchLatLon(expanded_location);
    String coords_string = String(coords.lat + "," + coords.lon);
    writeFile(LittleFS, "/coords.txt", coords_string.c_str());
  }
}