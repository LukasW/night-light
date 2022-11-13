#include <deepsleep.h>
#include "WiFi.h"
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>
#include <driver/rtc_io.h>

void goToDeepSleep(unsigned int deepSleepInSeconds, gpio_num_t pin)
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  adc_power_off();
  esp_wifi_stop();
  esp_bt_controller_disable();

  if (deepSleepInSeconds > 0)
  {
    // Configure the timer to wake us up!
    esp_sleep_enable_timer_wakeup(deepSleepInSeconds * 1000000L);
  }
  rtc_gpio_pullup_en(pin);
  esp_sleep_enable_ext0_wakeup(pin, 0);

  // Go to sleep! Zzzz
  esp_deep_sleep_start();
}
