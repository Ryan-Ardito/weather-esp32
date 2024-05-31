#include "heltec.h"
#include "Arduino.h"

#include "weathertypes.h"
#include "wifi.h"

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED, RST_OLED);

void showInvalidLocation(void)
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Invalid location!");
  display.drawString(0, 12, "rebooting...");
  display.display();
}

void showConnecting(void)
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Connecting...");
  display.display();
}

// Turn LED on then off with a delay.
void delayBlink(int duration_millis)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(duration_millis / 2);
  digitalWrite(LED_BUILTIN, LOW);
  delay(duration_millis / 2);
}

void LedON(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void LedOFF(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
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