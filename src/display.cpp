#include "heltec.h"
#include "Arduino.h"
// #include <WiFiManager.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include <LittleFS.h>
// #include <FS.h>

#include "consts.h"
// #include "utils.h"
#include "weathertypes.h"
// #include "persist.h"
// #include "owm_api.h"

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED, RST_OLED);

void SetupDisplay(void)
{
  display.init();
  display.setBrightness(255); // 0 is dim but visible, 255 is max bright
}

void showSetupInstructions(void)
{
  display.clear();

  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, String("SSID: " + String(ap_name)));
  display.drawString(0, 12, String("Pass: " + String(ap_password)));
  display.drawString(0, 24, "URL:  192.168.4.1");

  display.display();
}

void displayWeather(WeatherInfo info)
{
  if (!info.temperature)
  {
    return;
  }

  display.clear();

  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, String(info.location));
  display.drawString(86, 0, info.clock_time);

  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 19, String(String(info.temperature, 1) + "°"));
  display.drawString(75, 19, String(String(info.humidity) + "%"));

  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 45, String(info.description));

  display.display();
}