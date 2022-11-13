#ifndef __PIXELS_H__
#define __PIXELS_H__

void setupPixels();

void setAllPixelsColor(uint8_t red, uint8_t green, uint8_t blue);

void setPixelsColor(uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue);

void teardownPixels();

void loopPixels();

#endif