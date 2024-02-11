// #include <Adafruit_SSD1331.h>

class SSD1331_Display : public Display {
 public:
  SSD1331_Display(Adafruit_SSD1331 &display) : _display(display) {}

  void drawPixel(int16_t x, int16_t y, uint16_t color) override { _display.drawPixel(x, y, color); }

  void startWrite(void) override { _display.startWrite(); }
  void endWrite(void) override { _display.endWrite(); }
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override {
    _display.fillRect(x, y, w, h, color);
  }
  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h,
                  uint16_t color) override {
    _display.drawBitmap(x, y, bitmap, w, h, color);
  }
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override {
    _display.drawFastHLine(x, y, w, color);
  }
  void display() override { }

 private:
  Adafruit_SSD1331 &_display;
};