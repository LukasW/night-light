#include <Arduino.h>
#include <deepsleep.h>
#include <log.h>
#include <wlan.h>
#include <WebServer.h>
#include "settings.h"
#include <pixels.h>
#include <ArduinoJson.h>
#include <string>
#include <sstream>
#include <time.h>

WebServer server(80);
void printLocalTime();

enum PixelsStatus
{
    OFF,
    COLOR,
    COLOR_RED,
    COLOR_BLUE,
    COLOR_GREEN,
    ANIMATION
};

volatile PixelsStatus pixelsStatus = ANIMATION;
volatile bool changed = true;
volatile uint8_t red;
volatile uint8_t green;
volatile uint8_t blue;
volatile int16_t pixel = -1;

const char *ntpServer = "pool.ntp.org";

void webserver(void *parameter)
{
    sleep(2);
    setupWiFi(WLAN_SSID, WLAN_HOSTNAME, WLAN_PASSWORD);

    configTime(0, 0, ntpServer);
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // Zurich
    tzset();

    printLocalTime();

    server.on("/color", HTTP_GET, []()
              {
        pixelsStatus = COLOR;
        changed = true; 
        red = (uint8_t)server.arg("red").toInt();
        green = (uint8_t)server.arg("green").toInt();
        blue  = (uint8_t)server.arg("blue").toInt();
        if (server.hasArg("pixel")) {
            pixel = (uint8_t)server.arg("pixel").toInt();
        } else {
            pixel = -1;
        }
        server.send(200); });

    server.on("/animation", HTTP_GET, []()
              {
        pixelsStatus = ANIMATION; 
        changed = true; server.send(200); });

    server.on("/off", HTTP_GET, []()
              {                 
       pixelsStatus = OFF;
       changed = true;server.send(200); });

    server.begin();

    for (;;)
    {
        server.handleClient();
    }

    vTaskDelete(NULL);
}

long lastIsr = millis();

void IRAM_ATTR ISR()
{
    if (millis() - lastIsr > 200)
    {
        switch (pixelsStatus)
        {
        case ANIMATION:
            pixelsStatus = COLOR_BLUE;
            break;
        case COLOR:
            pixelsStatus = OFF;
            break;
        case COLOR_BLUE:
            pixelsStatus = COLOR_RED;
            break;
        case COLOR_RED:
            pixelsStatus = COLOR_GREEN;
            break;
        case COLOR_GREEN:
            pixelsStatus = OFF;
            break;
        case OFF:
            pixelsStatus = ANIMATION;
            break;
        }

        changed = true;

        lastIsr = millis();
    }
}

const int BUTTON_PIN = 0;

void setup()
{
    Serial.begin(115200);
    Log::LOG.enable();

    pinMode(GPIO_NUM_0, INPUT_PULLUP);

    attachInterrupt(GPIO_NUM_0, ISR, RISING);

    /* we create a new task here */
    xTaskCreate(
        webserver,      /* Task function. */
        "another Task", /* name of task. */
        10000,          /* Stack size of task */
        NULL,           /* parameter of the task */
        1,              /* priority of the task */
        NULL);          /* Task handle to keep track of created task */

    Log::LOG.info("Started ...");
}

void loop()
{
    if (changed)
    {
        changed = false;
        switch (pixelsStatus)
        {
        case ANIMATION:
            Log::LOG.info("Switch to Animation");
            setupPixels();
            break;
        case COLOR:
            Log::LOG.info("Switch to Color");
            setAllPixelsColor(red, green, blue);
            break;
        case COLOR_BLUE:
            Log::LOG.info("Switch to Color Blue");
            setAllPixelsColor(0, 0, 0xff);
            break;
        case COLOR_RED:
            Log::LOG.info("Switch to Color Red");
            setAllPixelsColor(0xff, 0, 0);
            break;
        case COLOR_GREEN:
            Log::LOG.info("Switch to Color Green");
            setAllPixelsColor(0, 0xff, 0);
            break;
        case OFF:
            Log::LOG.info("Switch to Off");
            teardownPixels();
            break;
        }
    }

    if (pixelsStatus == ANIMATION)
    {
        loopPixels();
    }
}

void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time 1");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}