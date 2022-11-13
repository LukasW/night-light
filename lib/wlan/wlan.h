#ifndef __wlan_h__
#define __wlan_h__

#include <Arduino.h>
#include <string>

using std::string;

void setupWiFi(const string &ssid, const string &hostname, const string &password);

#endif // __wlan_h__

