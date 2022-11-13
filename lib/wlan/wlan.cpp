#include <WiFi.h>
#include <wlan.h>
#include <log.h>

using std::string;

void setupWiFi(const string &ssid, const string &hostname, const string &password)
{
  // We start by connecting to a WiFi network
  Log::LOG.info("Connecting to " + ssid);

  WiFi.mode(WIFI_MODE_STA);
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(ssid.c_str(), password.c_str());

  int retry = 10;
  while (retry > 0)
  {
    if (WiFi.status() == WL_CONNECTED)
      break;
    Log::LOG.info("Failed! Retry in 5 seconds.");
    delay(5000);
    retry--;
  }
  if (retry == 0)
  {
    Log::LOG.info("Failed to connect to WiFi. Sleep one minute and restart.");
    delay(60000);
    ESP.restart();
  }

  Log::LOG.info("WiFi connected");
  Log::LOG.info(string("IP address: ") + WiFi.localIP().toString().c_str());
}
