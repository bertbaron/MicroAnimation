#ifndef animation_h
#define animation_h
#include <Adafruit_GFX.h>

#include "Arduino.h"

class MicroAnimation
{
public:

    /*!
     * @brief Creates a new animation object.
     * @param data The animation data.
     * @param display The display on which the animation is to be drawn.
     * @param x The x-position of the animation on the display.
     * @param y The y-position of the animation on the display.
     * @param color The color in which the animation is drawn.
     */
  MicroAnimation(const uint8_t *data, Adafruit_GFX *display, uint16_t x = 0, uint16_t y = 0,
                 uint16_t color = 1);

  /*!
   * @brief Get the number of frames in the animation.
   */
  int getFrameCount();

  /*!
   * @brief Get the width of the animation.
   */
  int getWidth();

    /*!
     * @brief Get the height of the animation.
     */
  int getHeight();

    /*!
     * @brief Draws the specified frame. Note that delta frames can only be drawn in order starting from frame 0 or
     another non-delta frame. Other frames can be drawn in any order.
     */
  void drawFrame(int frameNumber);

    /*!
     * @brief Set the frame rate of the animation.
     * @param fps The frame rate in frames per second.
     */
  void setFrameRate(uint16_t fps);

    /*!
     * @brief Sets the position of the animation on the display.
     */
  void setPosition(int16_t x, int16_t y);

    /*!
     * @brief Sets the color in which the animation is drawn.
     */
  void setColor(uint16_t color);

    /*!
     * @brief Sets the background color of the animation.
     */
  void setBackgroundColor(uint16_t color);

  /*!
   * @brief Play the animation, blocks until done.
   */
  void play();

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
   * @brief Pauses the animation, pauze(false) will resume the animation.
   */
  void pause(bool pause = true);

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
   * @brief Check if the animation is pauzed.
   */
  bool isPaused();

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
  bool _paused;
  void (*_finishedCallback)();

  void _animationFinished();
  void _reset(bool finished);
};

#endif