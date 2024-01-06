#ifndef animation_h
#define animation_h
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
   * @brief Updates the animation, potentially drawing a new frame. 
   *        Does nothing if the animation is not running. 
   *        This is typically called in the loop() function.
   * @return True if the animation is running. 
   */
  bool update();

  /*!
   * @brief Check if the animation is finished and resets the 'finished' flag.
   */
  bool finished();

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