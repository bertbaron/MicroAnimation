#include "MicroAnimation.h"

#include <Arduino.h>

void doDrawFrame(const uint8_t *data, Display &display, int16_t x, int16_t y, int16_t w,
                 int16_t h, uint16_t bgColor, uint16_t color);
void doDrawRleFrame(const uint8_t *data, Display &display, int16_t x, int16_t y, int16_t w,
                    int16_t h, uint16_t bgColor, uint16_t color);

int MicroAnimation::getFrameCount() { return pgm_read_byte(_data + 1); }

int MicroAnimation::getWidth() { return pgm_read_byte(_data + 2); }

int MicroAnimation::getHeight() { return pgm_read_byte(_data + 3); }

void MicroAnimation::setPosition(int16_t x, int16_t y) {
  _x = x;
  _y = y;
}

void MicroAnimation::setColor(uint16_t color) { _color = color; }
void MicroAnimation::setBackgroundColor(uint16_t color) { _backgroundColor = color; }
void MicroAnimation::setFrameRate(uint16_t fps) { _frameDelay = 1000 / fps; }

void MicroAnimation::drawFrame(int frameNumber) {
  uint16_t dataOffset = pgm_read_word(_data + 4 + frameNumber * 2);
  doDrawFrame(_data + dataOffset, _display, _x, _y, getWidth(), getHeight(), _backgroundColor,
              _color);
  _display.display();
}

void MicroAnimation::play() {
  for (int i = 0; i < getFrameCount(); i++) {
    uint16_t exp = millis() + _frameDelay;
    drawFrame(i);

    uint16_t now = millis();
    if (exp - now < _frameDelay) {  // overflow-safe
      delay(exp - now);
    }
  }
}

void MicroAnimation::start(bool loop) {
  _frame = 0;
  _lastFrameTime = millis();
  _finished = false;
  _loop = loop;
}

bool MicroAnimation::update() {
  _finished = false;
  if (_frame < 0 || _paused) {
    return false;
  }
  bool result = false;
  uint16_t now = millis();
  if (now - _lastFrameTime >= _frameDelay) {
    drawFrame(_frame++);
    _lastFrameTime = now;
    result = true;
    if (_frame >= getFrameCount()) {
      if (_loop) {
        _frame = 0;
      } else {
        if (_abort) {
          _reset(false);
        } else {
          _animationFinished();
        }
      }
    }
  }
  return result;
}

void MicroAnimation::stop(bool waitForLoopCycle) {
  if (waitForLoopCycle) {
    _loop = false;
  } else if (_frame >= 0) {
    _animationFinished();
  }
}

void MicroAnimation::abort(bool waitForLoopCycle) {
  if (waitForLoopCycle) {
    _loop = false;
    _abort = true;
  } else {
    _reset(false);
  }
}

void MicroAnimation::pause(bool pause) {
   _paused = pause;
}

void MicroAnimation::_animationFinished() {
  if (_finishedCallback != NULL) {
    _finishedCallback();
  }
  _reset(true);
}

void MicroAnimation::_reset(bool finished) {
  _frame = -1;
  _finished = finished;
  _loop = false;
  _abort = false;
  _paused = false;
}

bool MicroAnimation::isRunning() { return _frame >= 0 && !_paused; }

bool MicroAnimation::isPaused() { return _paused; }

bool MicroAnimation::isFinished() { return _finished; }

void MicroAnimation::onFinish(void (*finishedCallback)()) { _finishedCallback = finishedCallback; }

#define BG 0
#define FG 1
#define TRANSPARENT 2

#define FRAME_TYPE_MASK 0xC0
#define RLE_INITIAL_COLOR_MASK 0x03
#define FRAME_TYPE_UNCOMPRESSED 0x00
#define FRAME_TYPE_RLE 0x40
#define FRAME_TYPE_RLE_DELTA 0x80
#define MULTIBYTE_FLAG 0x80

/*
 * Frame format:
 * 1 byte: frame type | initial color
 */
void doDrawFrame(const uint8_t *data, Display &display, int16_t x, int16_t y, int16_t w,
                 int16_t h, uint16_t bgColor, uint16_t color) {
  uint8_t type = pgm_read_byte(data);
  if ((type & FRAME_TYPE_MASK) == FRAME_TYPE_UNCOMPRESSED) {
    display.fillRect(x, y, w, h, bgColor);
    display.drawBitmap(x, y, data + 1, w, h, color);
  } else {
    doDrawRleFrame(data, display, x, y, w, h, bgColor, color);
  }
}

void doDrawRleFrame(const uint8_t *data, Display &display, int16_t x, int16_t y, int16_t w,
                    int16_t h, uint16_t bgColor, uint16_t drawColor) {
  uint8_t type = pgm_read_byte(data);
  const bool isDelta = (type & FRAME_TYPE_MASK) == FRAME_TYPE_RLE_DELTA;
  uint8_t lastColor = type & RLE_INITIAL_COLOR_MASK;
  int16_t startPixel = 0;
  uint16_t idx = 1;

  display.startWrite();

  if (!isDelta) {
    display.fillRect(x, y, w, h, bgColor);
  }

  while (startPixel < w * h) {
    uint8_t value = pgm_read_byte(data + idx++);
    uint8_t color = isDelta ? (lastColor + 1 + ((value >> 6) & 0x01)) % 3 : 1 - lastColor;
    uint16_t runLength = isDelta ? value & 0x3F : value & 0x7F;
    while (value & MULTIBYTE_FLAG) {
      value = pgm_read_byte(data + idx++);
      runLength = (runLength << 7) | (value & 0x7F);
    }
    if (runLength == 0) {
      uint8_t escapedByte = 0;
      do {
        escapedByte = pgm_read_byte(data + idx++);
        uint8_t bits = escapedByte;
        for (uint8_t i = 0; i < 7; i++) {
          color = bits & 0x40;
          if (color || isDelta) {
            uint16_t pixelColor = color ? drawColor : bgColor;
            display.drawPixel(x + (startPixel % w), y + (startPixel / w), pixelColor);
          }
          bits <<= 1;
          startPixel++;
        }
      } while (escapedByte & MULTIBYTE_FLAG);
      lastColor = color ? FG : BG;
      continue;
    }
    if (color == FG || (isDelta && color == BG)) {
      uint16_t pixelColor = color == FG ? drawColor : bgColor;
      uint16_t index = startPixel;
      uint16_t remainingLength = runLength;
      while (remainingLength > 0) {
        uint16_t lineLength = min(remainingLength, uint16_t(w - (index % w)));
        display.drawFastHLine(x + (index % w), y + (index / w), lineLength, pixelColor);
        index += lineLength;
        remainingLength -= lineLength;
      }
    }
    lastColor = color;
    startPixel += runLength;
  }
  display.endWrite();
}