// wifi.h
#ifndef WIFI_H
#define WIFI_H

#include <WiFiManager.h>

extern char *const ap_name;
extern char *const ap_password;

void saveConfigCallback(void);
void configModeCallback(WiFiManager *myWiFiManager);
void setupWifiConfig(void);

#endif