// wifi.h
#ifndef WIFI_H
#define WIFI_H

#include <WiFiManager.h>

void saveConfigCallback(void);
void configModeCallback(WiFiManager *myWiFiManager);
void setupWifiConfig(void);

#endif