#ifndef compressed_animation_h
#define compressed_animation_h
#include <Adafruit_GFX.h>

#include "Arduino.h"

class Animation {
 public:
  Animation(const uint8_t *data, Adafruit_GFX *display, uint16_t x = 0, uint16_t y = 0,
            uint16_t color = 1);
            
  int getFrameCount();
  int getWidth();
  int getHeight();
  void drawFrame(int frameNumber);

  void setFrameDelay(uint16_t delay);
  void setPosition(uint16_t x, uint16_t y);
  void setColor(uint16_t color);
  void setBackgroundColor(uint16_t color);

  /*!
   * @brief Play the animation, blocks until done.
   */
  void play();

  /*
   Functions for non-blocking animation 
   */

  void start();
  /*!
   * @brief Draw the next frame of the animation.
   * @return True if the animation is still running. 
   */
  bool run();
  /*!
   * @brief Check if the animation is finished and resets the 'finished' flag.
   */
  bool isFinished();

  /*!
   * @brief Specifies a callback-function that is called when the animation is finished.
   */
  void onFinish(void (*finishedCallback)());

 private:
  const uint8_t *_data;
  Adafruit_GFX *_display;
  uint16_t _x, _y;
  uint16_t _color;
  uint16_t _backgroundColor;
  uint16_t _frameDelay;

  // state for non-blocking animation
  int _frame;
  uint32_t _lastFrameTime;
  bool _finished;
  void (*_finishedCallback)();

  void _animationFinished();
};

#endif