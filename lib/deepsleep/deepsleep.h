#include <driver/gpio.h>

#ifndef __deepsleep_h__
#define __deepsleep_h__

void goToDeepSleep(unsigned int deepSleepInSeconds, gpio_num_t pin);

#endif // __deepsleep_h__
