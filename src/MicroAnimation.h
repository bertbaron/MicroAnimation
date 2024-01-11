#ifndef animation_h
#define animation_h
#include <Adafruit_GFX.h>

#include "Arduino.h"

class MicroAnimation
{
public:
  MicroAnimation(const uint8_t *data, Adafruit_GFX *display, uint16_t x = 0, uint16_t y = 0,
                 uint16_t color = 1);

  int getFrameCount();
  int getWidth();
  int getHeight();
  void drawFrame(int frameNumber);

  void setFrameDelay(uint16_t delay);
  void setPosition(int16_t x, int16_t y);
  void setColor(uint16_t color);
  void setBackgroundColor(uint16_t color);

  /*!
   * @brief Play the animation, blocks until done.
   */
  void play();

  /*
   Functions for non-blocking animation
   */

  /*!
   * @brief Starts the animation.
   * @param loop If true, the animation will keep looping until stop() is called.
   */
  void start(bool loop = false);

  /*!
   * @brief Stops the animation, optionally waiting for the current loop cycle to finish.
   *        The animation will be considered finished and the optional callback will be called.
   * @param waitForLoopCycle If true, the animation will stop after the current loop cycle has finished.
   */
  void stop(bool waitForLoopCycle = false);

  /*!
   * @brief Stops the animation, optionally waiting for the current loop cycle to finish.
   *        The finished flag will not be set and the optional callback will not be called.
   * @param waitForLoopCycle If true, the animation will stop after the current loop cycle has finished.
   */
  void abort(bool waitForLoopCycle = false);

  /*!
   * @brief Updates the animation, potentially drawing a new frame.
   *        Does nothing if the animation is not running.
   *        This is typically called in the loop() function.
   * @return True if the animation is running.
   */
  bool update();

  /*!
   * @brief Check if the animation is running.
   */
  bool isRunning();

  /*!
   * @brief Check if the animation is finished normally (not aborted) during the last update() call
   */
  bool isFinished();

  /*!
   * @brief Specifies a callback-function that is called when the animation is finished.
   */
  void onFinish(void (*finishedCallback)());

private:
  const uint8_t *_data;
  Adafruit_GFX *_display;
  int16_t _x, _y;
  uint16_t _color;
  uint16_t _backgroundColor;
  uint16_t _frameDelay;

  // state for non-blocking animation
  int _frame;
  uint32_t _lastFrameTime;
  bool _finished;
  bool _loop;
  bool _abort;
  void (*_finishedCallback)();

  void _animationFinished();
  void _reset(bool finished);
};

#endif