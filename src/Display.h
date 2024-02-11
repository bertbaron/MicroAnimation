#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <stdint.h>

class Display {
 public:
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

  virtual void startWrite(void);
  virtual void endWrite(void);
  virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h,
                  uint16_t color);
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void display() = 0;
};

#endif  // DISPLAY_INTERFACE_H