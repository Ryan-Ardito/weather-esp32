#include "Arduino.h"
#include <ArduinoJson.h>

#include <unordered_map>

#include "weathertypes.h"

// chatGPT
String getStateFullName(const String &stateCode)
{
  // Define a mapping of 2-letter state codes to their full names
  std::unordered_map<std::string, std::string> stateMap = {
      {"al", "alabama"},
      {"ak", "alaska"},
      {"az", "arizona"},
      {"ar", "arkansas"},
      {"ca", "california"},
      {"co", "colorado"},
      {"ct", "connecticut"},
      {"de", "delaware"},
      {"fl", "florida"},
      {"ga", "georgia"},
      {"hi", "hawaii"},
      {"id", "idaho"},
      {"il", "illinois"},
      {"in", "indiana"},
      {"ia", "iowa"},
      {"ks", "kansas"},
      {"ky", "kentucky"},
      {"la", "louisiana"},
      {"me", "maine"},
      {"md", "maryland"},
      {"ma", "massachusetts"},
      {"mi", "michigan"},
      {"mn", "minnesota"},
      {"ms", "mississippi"},
      {"mo", "missouri"},
      {"mt", "montana"},
      {"ne", "nebraska"},
      {"nv", "nevada"},
      {"nh", "new Hampshire"},
      {"nj", "new Jersey"},
      {"nm", "new Mexico"},
      {"ny", "new York"},
      {"nc", "north Carolina"},
      {"nd", "north Dakota"},
      {"oh", "ohio"},
      {"ok", "oklahoma"},
      {"or", "oregon"},
      {"pa", "pennsylvania"},
      {"ri", "rhode Island"},
      {"sc", "south Carolina"},
      {"sd", "south Dakota"},
      {"tn", "tennessee"},
      {"tx", "texas"},
      {"ut", "utah"},
      {"vt", "vermont"},
      {"va", "virginia"},
      {"wa", "washington"},
      {"wv", "west Virginia"},
      {"wi", "wisconsin"},
      {"wy", "wyoming"}
  };

  // Convert the input state code to lowercase for case-insensitive comparison
  String lowercaseStateCode = stateCode;
  lowercaseStateCode.toLowerCase();

  // Check if the state code is in the map
  auto it = stateMap.find(lowercaseStateCode.c_str());
  if (it != stateMap.end())
  {
    // Return the full name if found
    return String(it->second.c_str());
  }
  else
  {
    // Return the original string if not a 2-letter state code
    return stateCode;
  }
}

String expandCityStateString(const String &cityState)
{
  // Convert the input string to lowercase
  String lowercaseCityState = cityState;
  lowercaseCityState.toLowerCase();

  // Find the position of the comma
  int commaPos = lowercaseCityState.indexOf(',');

  // Extract city and state substrings
  String city = lowercaseCityState.substring(0, commaPos);
  String state = lowercaseCityState.substring(commaPos + 1);

  // Remove whitespace from city and state
  city.trim();
  state.trim();

  // Run the state through getStateFullName function
  String fullState = getStateFullName(state);

  // Concatenate the results with a comma
  return city + "," + fullState;
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