#include "Animation.h"

#include <Adafruit_GFX.h>
#include <Arduino.h>

#if __has_include(<Adafruit_SSD1306.h>)
#include <Adafruit_SSD1306.h>
#endif

void doDrawFrame(const uint8_t* data, Adafruit_GFX* display, int16_t x, int16_t y, int16_t w,
                 int16_t h, uint16_t bgColor, uint16_t color);
void doDrawRleFrame(const uint8_t* data, Adafruit_GFX* display, int16_t x, int16_t y, int16_t w,
                    int16_t h, uint16_t bgColor, uint16_t color);

Animation::Animation(const uint8_t* data, Adafruit_GFX* display, uint16_t x, uint16_t y,
                     uint16_t color) {
  _data = data;
  _display = display;
  _x = x;
  _y = y;
  _color = color;
  _backgroundColor = 0;
  _frameDelay = 42;
  _frame = -1;
  _finished = false;
  _finishedCallback = NULL;
  _lastFrameTime = 0;
}

int Animation::getFrameCount() { return pgm_read_byte(_data); }

int Animation::getWidth() { return pgm_read_byte(_data + 1); }

int Animation::getHeight() { return pgm_read_byte(_data + 2); }

void Animation::setPosition(uint16_t x, uint16_t y) {
  _x = x;
  _y = y;
}

void Animation::setColor(uint16_t color) { _color = color; }
void Animation::setBackgroundColor(uint16_t color) { _backgroundColor = color; }
void Animation::setFrameDelay(uint16_t delay) { _frameDelay = delay; }

void Animation::drawFrame(int frameNumber) {
  uint16_t dataOffset = pgm_read_word(_data + 3 + frameNumber * 2);
  doDrawFrame(_data + dataOffset, _display, _x, _y, getWidth(), getHeight(), _backgroundColor,
              _color);
#ifdef _Adafruit_SSD1306_H_
    ((Adafruit_SSD1306*)_display)->display();
#endif
}

void Animation::play() {
  for (int i = 0; i < getFrameCount(); i++) {
    uint16_t exp = millis() + _frameDelay;
    drawFrame(i);

    uint16_t now = millis();
    if (now < exp) {
      delay(exp - now);
    }
  }
}

void Animation::start() {
  _frame = 0;
  _lastFrameTime = millis();
  _finished = false;
}

void Animation::_animationFinished() {
  _frame = -1;
  _finished = true;
  if (_finishedCallback != NULL) {
    _finishedCallback();
  }
}

bool Animation::update() {
  if (_frame < 0) {
    return false;
  }
  uint16_t now = millis();
  if (now - _lastFrameTime >= _frameDelay) {
    drawFrame(_frame++);
    _lastFrameTime = now;
    if (_frame >= getFrameCount()) {
      _animationFinished();

    }
  }
  return !_finished;
}

bool Animation::finished() {
  bool finished = _finished;
  _finished = false;
  return finished;
}

void Animation::onFinish(void (*finishedCallback)()) { _finishedCallback = finishedCallback; }

#define BG 0
#define FG 1
#define TRANSPARENT 2

#define FRAME_TYPE_MASK 0b11000000
#define RLE_INITIAL_COLOR_MASK 0b00000011
#define FRAME_TYPE_UNCOMPRESSED 0
#define FRAME_TYPE_RLE 0b01000000
#define FRAME_TYPE_RLE_DELTA 0b10000000
#define MULTIBYTE_FLAG 0b10000000

/*
 * Frame format:
 * 1 byte: frame type | initial color
 */
void doDrawFrame(const uint8_t* data, Adafruit_GFX* display, int16_t x, int16_t y, int16_t w,
                 int16_t h, uint16_t bgColor, uint16_t color) {
  uint8_t type = pgm_read_byte(data);
  if ((type & FRAME_TYPE_MASK) == FRAME_TYPE_UNCOMPRESSED) {
    display->fillRect(x, y, w, h, bgColor);
    display->drawBitmap(x, y, data + 1, w, h, color);
  } else {
    doDrawRleFrame(data, display, x, y, w, h, bgColor, color);
  }
}

void doDrawRleFrame(const uint8_t* data, Adafruit_GFX* display, int16_t x, int16_t y, int16_t w,
                    int16_t h, uint16_t bgColor, uint16_t drawColor) {
  uint8_t type = pgm_read_byte(data);
  const bool useDeltaCompression = (type & FRAME_TYPE_MASK) == FRAME_TYPE_RLE_DELTA;
  uint8_t lastColor = type & RLE_INITIAL_COLOR_MASK;
  int16_t startPixel = 0;
  uint16_t idx = 1;

  display->startWrite();

  if (!useDeltaCompression) {
    display->fillRect(x, y, w, h, bgColor);
  }

  while (startPixel < w * h) {
    uint8_t value = pgm_read_byte(data + idx++);
    uint8_t color =
        useDeltaCompression ? (lastColor + ((value >> 6) & 0x01) + 1) % 3 : 1 - lastColor;
    uint16_t runLength = useDeltaCompression ? value & 0b00111111 : value & 0b01111111;
    if (value & MULTIBYTE_FLAG) {
      uint8_t rlByte = pgm_read_byte(data + idx++);
      runLength = (runLength << 7) | (rlByte & 0b01111111);
      while (rlByte & MULTIBYTE_FLAG) {
        rlByte = pgm_read_byte(data + idx++);
        runLength = (runLength << 7) | (rlByte & 0b01111111);
      }
    }
    if (color == FG || (useDeltaCompression && color == BG)) {
      uint16_t pixelColor = color == FG ? drawColor : bgColor;
      uint16_t index = startPixel;
      uint16_t remainingLength = runLength;
      while (remainingLength > 0) {
        uint16_t lineLength = min(remainingLength, w - (index % w));
        display->drawFastHLine(x + (index % w), y + (index / w), lineLength, pixelColor);
        index += lineLength;
        remainingLength -= lineLength;
      }
    }
    lastColor = color;
    startPixel += runLength;
  }
  display->endWrite();
}