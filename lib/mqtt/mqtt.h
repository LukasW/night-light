#include "sensor.h"
#include <stdint.h>
#include <string>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <measurement.h>

#ifndef __mqtt_h__
#define __mqtt_h__

class Mqtt
{
public:
    Mqtt();

    bool connect();
    void disconnect();

    void sendStatusMessage(std::string &message);
    void sendSensorData(Sample &sample);

    bool loop();
private:
    PubSubClient mqttClient;
    WiFiClient wifiClient;

    int str2int(const char *str, unsigned int len);
    void mqttCommandHandler(char *topic, byte *payload, unsigned int length);
};

#endif // __mqtt_h__
