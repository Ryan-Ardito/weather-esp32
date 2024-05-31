#include "Arduino.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "weathertypes.h"
#include "secrets.h"

const String owm_base_url = "https://api.openweathermap.org/data/2.5/weather";
const String apikey = OWM_API_KEY;

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
  Coords coords;
  String url = String("http://api.openweathermap.org/geo/1.0/direct?q=" + city_state + ",us" + "&limit=5&appid=" + apikey);
  String json = apiGet(url);
  // janky
  if (json == String("[]"))
  {
    coords.lat = String("err");
    return coords;
  }
  JsonDocument doc;
  const auto err = deserializeJson(doc, json);
  String lat = doc[0]["lat"];
  String lon = doc[0]["lon"];
  coords.lat = lat;
  coords.lon = lon;

  return coords;
}

String fetchWeatherInfo(Coords coords)
{
  String weather_api_url = String(owm_base_url + "?units=imperial" + "&lat=" + coords.lat + "&lon=" + coords.lon + "&appid=" + apikey);
  return apiGet(weather_api_url);
}