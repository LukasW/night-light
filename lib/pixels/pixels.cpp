// NeoPixelFunLoop
// This example will move a trail of light around a series of pixels.
// A ring formation of pixels looks best.
// The trail will have a slowly fading tail.
//
// This will demonstrate the use of the RotateRight method.
//

#include <NeoPixelBus.h>

const uint16_t PixelCount = 50; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 4;    // make sure to set this to the correct pin, ignored for Esp8266

NeoGamma<NeoGammaTableMethod> colorGamma; // for any fade animations, best to correct gamma

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

void setupPixels()
{
    strip.Begin();
    strip.Show();
}

void loopPixels()
{
    unsigned long t = millis();

    HslColor red = RgbColor(0, 0, 255);
    for (uint16_t index = 0; index < strip.PixelCount(); index++)
    {
        float lightness = (sin(2 * PI * index / 50 + t * 2 * PI / 10000.0) + 1.0) / 4.0;
        RgbColor color = HslColor(red.H, red.S, lightness);
        strip.SetPixelColor(index, colorGamma.Correct(color));
    }

    strip.Show();
}

void setPixelsColor(uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    RgbColor color(red, green, blue);
    strip.SetPixelColor(pixel, color);
    strip.Show();
    sleep(1);
}

void setAllPixelsColor(uint8_t red, uint8_t green, uint8_t blue)
{
    RgbColor color(red, green, blue);
    for (uint16_t index = 0; index < strip.PixelCount(); index++)
    {
        strip.SetPixelColor(index, color);
    }
    strip.Show();
    sleep(1);
}

void teardownPixels()
{
    setAllPixelsColor(0, 0, 0);
}