#include <mqtt.h>
#include "settings.h"

Mqtt::Mqtt()
{
  mqttClient.setClient(wifiClient);
}

bool Mqtt::connect()
{
  int wifiClientState = wifiClient.connect(MQTT_SERVER, 1883);
  if (!wifiClientState)
  {
    LOG("Failed to connect to server, state=");
    LOG_LN(wifiClientState);
    return false;
  }

  if (!mqttClient.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASSWORD))
  {
    LOG("Failed to connect to mqtt, state=");
    LOG_LN(mqttClient.state());
    return false;
  }

  return true;
}

void Mqtt::disconnect()
{
  mqttClient.disconnect();
  wifiClient.stop();
}

void Mqtt::sendStatusMessage(std::string &message)
{
  boolean connected = mqttClient.connected();
  if (!connected)
  {
    if (!connect())
    {
      LOG("Failed to send status: ");
      LOG_LN(message.c_str());
      return;
    }
  }
  mqttClient.publish(STATUS_CHANNEL, message.c_str());
  if (!connected)
  {
    disconnect();
  }
}

void Mqtt::sendSensorData(Sample &sample)
{
  char message[255];

  sprintf(message, "{\n\t\"temp\": %f,\n\t\"hum\": %f,\n\t\"pr\": %f,\n\t\"bat\": %f,\n\t\"soil\": %f\n}",
          sample.getTemperature(),
          sample.getHumidity(),
          sample.getPressure() / 100.0,
          sample.getBatteryVoltage(),
          sample.getSoilMoistureVoltage());

  LOG_LN("Sending message: ");
  LOG_LN(message);

  mqttClient.publish(DATA_CHANNEL, message);
}

bool Mqtt::loop()
{
  return mqttClient.loop();
}

int Mqtt::str2int(const char *str, unsigned int len)
{
  int i;
  int ret = 0;
  for (i = 0; i < len; ++i)
  {
    if (str[i] == '.')
    {
      break;
    }
    ret = ret * 10 + (str[i] - '0');
  }
  return ret;
}
