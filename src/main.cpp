#include "heltec.h"
#include "Arduino.h"
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <FS.h>

const int kSerialBaudRate = 115200;
const int kRequestIntervalMillis = 10 * 60 * 1000;

const char *ap_name = "Weather Daddy";
const char *ap_password = "password";
const String owm_base_url = "https://api.openweathermap.org/data/2.5/weather";
const String apikey = "9301cc18f16f719750a35990eb761e9e";
// const String lat = "38";
// const String lon = "-106";

bool force_config;
bool should_save_config;

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED, RST_OLED);

struct Coords
{
  String lat;
  String lon;
};

Coords coords;

struct WeatherInfo
{
  String location;
  String description;
  String clock_time;
  float temperature;
  int humidity;
};

String readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while (file.available())
  {
    fileContent += String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- write failed");
  }
  file.close();
}

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

void SetupDisplay(void)
{
  display.init();
  display.setBrightness(255); // 0 is dim but visible, 255 is max bright
}

void delayBlink(int duration_millis)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(duration_millis / 2);
  digitalWrite(LED_BUILTIN, LOW);
  delay(duration_millis / 2);
}

String readConfig(void)
{
  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS mount failed");
    return "";
  }
  else
  {
    Serial.println("LittleFS mount success");
  }

  return readFile(LittleFS, "/data.txt");
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

String unixTimeToString(int unix_time, int zone_offset)
{
  int zone_time = unix_time + zone_offset;
  int hrs = (zone_time / 3600) % 12;
  if (hrs == 0)
  {
    hrs = 12;
  }
  String hrs_string = String(hrs);
  if (hrs < 10)
  {
    hrs_string = String(" " + hrs_string);
  }

  int mins = (zone_time / 60) % 60;
  String mins_string = String(mins);
  if (mins < 10)
  {
    mins_string = String("0" + mins_string);
  }

  return String(hrs_string + ":" + mins_string);
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

WeatherInfo parseJsonInfo(String payload)
{
  WeatherInfo info;

  JsonDocument doc;
  const auto err = deserializeJson(doc, payload);

  const auto main = doc["main"];
  const auto datetime = (const uint32_t)doc["dt"];
  const auto weather = doc["weather"][0];
  const auto timezone_offset = (const int32_t)doc["timezone"];
  info.clock_time = unixTimeToString(datetime, timezone_offset);
  info.location = (const char *)doc["name"];
  info.description = (const char *)weather["description"];
  info.temperature = (const float)main["temp"];
  info.humidity = (const uint8_t)main["humidity"];

  return info;
}

String apiGet(String url)
{
  String response_json;
  HTTPClient http;

  http.begin(url);

  int httpCode = http.GET();
  if (httpCode < 1)
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    return response_json;
  }

  Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  if (httpCode == HTTP_CODE_OK)
  {
    response_json = http.getString();
  }

  http.end();
  return response_json;
}

Coords fetchLatLon(String city_state)
{
  String url = String("http://api.openweathermap.org/geo/1.0/direct?q=" + city_state + ",us" + "&limit=5&appid=" + apikey);
  String json = apiGet(url);
  JsonDocument doc;
  const auto err = deserializeJson(doc, json);
  String lat = doc[0]["lat"];
  String lon = doc[0]["lon"];
  Coords coords;
  coords.lat = lat;
  coords.lon = lon;

  return coords;
}

String fetchWeatherInfo(Coords coords)
{
  String weather_api_url = String(owm_base_url + "?units=imperial" + "&lat=" + coords.lat + "&lon=" + coords.lon + "&appid=" + apikey);
  return apiGet(weather_api_url);
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