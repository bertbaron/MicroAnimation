#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <MicroAnimation.h>
#include <adapters/SSD1306_Display.h>

// Test it here (copy this code): https://wokwi.com/projects/386250018661621761

/*
 * This sketch is used to test the maximum number of Frames Per Second (FPS) on your hardware. You can test
 * for example with different sizes or with different compression options. This also gives an indication
 * of how much your program can spend on other tasks while still maintaining the desired FPS.
 * 
 * Note that the render-time of frames can differ significantly, so besides the average FPS, also the worst-case
 * FPS is shown, which is the maximum framerate at which all frames can be rendered in time.
 * 
 * Please note that the Wokwi simulator is not a good indication of the actual performance on real hardware.
 */

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RST_PIN -1
#define SCREEN_I2C_ADDR 0x3D
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST_PIN);

// Icons by https://icons8.com, converted using https://animator.wokwi.com/ and
// https://bertbaron.github.io/MicroAnimationConverter/

// clang-format off
const byte PROGMEM animationData[] = {0,28,32,32,60,0,173,0,198,0,246,0,43,1,108,1,174,1,237,1,52,2,108,2,160,2,213,2,254,2,58,3,117,3,179,3,235,3,25,4,64,4,118,4,167,4,220,4,24,5,88,5,151,5,217,5,35,6,126,6,65,48,3,28,5,25,0,49,25,0,33,25,0,49,27,5,28,3,27,3,27,7,24,2,5,1,23,2,6,2,19,0,153,7,19,0,230,27,14,0,132,241,79,11,0,141,177,97,11,0,137,161,61,11,0,139,163,7,14,3,0,49,19,0,140,97,25,3,3,2,19,0,130,99,18,0,130,49,19,0,140,115,18,0,137,91,18,0,145,25,18,0,179,9,18,1,0,204,19,17,1,0,136,27,17,4,7,3,19,2,8,2,44,129,131,88,15,68,67,40,2,93,68,15,1,103,66,129,2,22,71,2,193,20,1,96,1,119,129,131,20,21,76,19,2,10,68,80,75,4,2,77,0,23,5,68,18,65,65,78,15,6,22,0,131,48,192,66,24,71,66,87,96,65,66,90,0,51,56,1,96,65,128,64,10,109,129,131,51,66,84,73,2,67,17,2,65,75,66,80,66,0,176,121,80,67,66,6,69,80,65,1,96,67,3,66,58,67,28,0,104,94,3,92,66,3,21,72,2,2,27,66,25,1,91,1,192,75,45,129,129,114,27,5,28,3,192,88,1,96,65,9,21,66,1,71,66,84,0,216,51,82,1,0,177,99,17,65,65,71,70,17,65,65,74,66,25,1,97,25,70,66,32,24,2,5,66,28,3,90,1,3,21,0,8,96,65,24,0,27,27,1,192,87,129,129,112,29,126,1,94,1,72,23,2,0,49,87,96,65,29,0,180,19,83,66,1,73,65,83,73,1,67,19,76,20,71,66,90,95,0,49,90,0,115,28,3,65,23,72,66,25,0,113,82,0,153,89,83,0,182,19,82,0,152,27,92,3,192,76,129,47,68,27,67,65,194,26,65,36,66,25,1,70,23,72,66,86,67,0,69,86,1,9,65,85,1,65,70,67,85,3,28,1,192,104,24,3,4,65,83,0,134,17,82,0,140,121,82,0,141,73,83,0,142,19,89,0,27,92,3,192,77,129,129,15,65,34,66,128,120,1,192,65,65,24,0,4,93,65,31,65,65,70,23,2,0,35,84,0,13,6,22,2,0,39,88,3,28,1,70,27,68,66,30,66,25,67,67,87,0,59,21,0,134,25,82,0,132,121,87,3,0,19,89,0,27,89,0,27,92,3,192,78,129,51,65,28,2,194,86,0,11,27,1,71,25,71,25,71,25,0,91,89,96,1,2,31,3,65,27,69,25,0,121,87,2,0,73,83,0,132,19,23,70,1,66,89,0,27,89,0,27,89,0,11,92,3,192,79,129,130,77,65,29,0,32,90,65,128,65,65,30,1,1,25,0,13,27,0,21,91,3,31,65,62,3,90,2,92,2,4,26,1,2,6,25,0,121,89,0,121,89,0,9,89,0,11,92,3,192,80,129,130,13,1,192,123,0,11,28,65,65,94,65,65,94,65,30,0,47,24,0,27,30,66,30,66,62,66,28,2,2,89,0,19,89,0,51,88,0,25,89,0,25,89,0,9,89,0,9,92,4,128,80,129,130,105,0,13,27,0,108,93,65,63,65,27,67,65,98,65,59,1,97,66,58,66,30,66,128,65,66,28,2,2,24,0,9,89,0,9,92,4,192,81,129,51,27,3,62,4,95,1,193,91,1,91,67,66,90,0,59,24,0,20,91,68,29,68,65,91,0,43,25,66,30,66,30,0,103,25,0,77,25,0,77,25,0,75,24,0,37,25,0,103,24,0,55,24,0,19,27,68,192,82,129,130,17,28,6,56,67,28,68,29,1,2,26,0,52,85,0,130,55,23,70,65,90,0,116,89,0,18,93,1,3,31,65,30,1,91,0,11,31,65,28,65,65,89,65,0,77,25,0,29,25,0,23,28,28,67,30,1,52,130,48,94,1,5,129,119,67,28,68,29,1,93,66,31,1,0,55,25,67,66,85,0,133,115,22,67,0,89,29,67,89,0,71,63,65,30,1,95,1,95,1,1,25,0,61,25,0,45,18,0,134,101,18,0,132,71,22,68,29,66,52,129,129,112,67,57,71,128,86,66,31,2,95,67,26,66,30,66,35,0,25,27,1,4,23,66,129,0,0,59,18,0,134,57,23,2,0,105,18,0,140,73,18,0,137,77,18,0,153,7,20,68,69,2,22,66,117,129,130,44,1,94,1,122,0,4,93,65,32,1,192,69,1,2,23,1,193,5,1,97,65,30,1,92,1,100,65,25,0,27,18,0,179,9,18,0,162,11,28,1,2,128,76,129,130,106,1,96,65,128,126,70,3,22,73,66,128,125,66,25,65,6,24,69,2,2,27,66,35,21,72,2,93,66,20,0,27,27,1,96,1,118,129,130,47,1,123,1,94,2,2,27,2,3,27,2,3,26,3,90,0,131,115,88,1,0,73,88,2,6,25,70,25,101,67,57,0,73,88,69,2,2,25,1,3,35,21,72,2,2,27,66,31,67,192,76,129,130,47,68,128,120,1,95,1,95,1,94,2,68,66,89,0,45,90,67,67,90,69,27,67,1,91,1,70,26,68,67,128,64,23,70,2,2,27,66,35,21,4,7,22,65,6,192,79,129,129,114,28,4,28,3,192,92,2,94,1,2,26,0,18,94,67,28,2,91,0,59,28,67,91,192,67,1,95,1,66,26,68,67,128,64,19,0,136,83,22,0,115,20,0,131,4,87,6,4,128,76,129,47,68,27,67,65,193,30,30,192,95,2,95,66,31,2,91,0,91,25,0,75,28,68,27,1,4,26,70,30,66,30,27,69,27,2,29,0,51,89,0,39,89,0,109,89,0,73,89,65,0,59,88,1,0,107,92,3,192,78,129,51,65,128,91,65,34,66,128,89,69,128,121,66,4,66,24,65,66,88,0,130,39,23,66,0,39,25,0,61,25,0,61,27,1,4,29,27,69,65,25,0,51,91,95,1,93,0,9,89,0,27,89,0,19,91,5,89,0,13,93,2,192,80,129,128,80,2,193,123,5,93,3,88,0,36,98,65,25,66,29,0,50,84,0,132,93,18,0,132,95,22,67,0,83,22,0,51,27,65,67,95,1,91,67,66,91,67,66,90,0,57,25,0,59,27,66,65,95,1,90,0,23,27,67,192,81,129,130,44,66,61,66,38,65,20,0,19,22,0,130,34,85,0,146,19,18,0,150,57,18,0,150,39,18,0,156,35,21,66,67,124,0,115,25,0,89,25,0,89,24,65,0,89,24,65,0,27,24,65,0,27,24,0,93,25,0,87,24,0,57,128,79,129,128,80,65,130,27,6,2,22,65,0,24,87,65,0,88,86,0,27,6,66,19,0,200,51,18,65,0,177,99,17,65,0,177,95,17,0,248,67,19,67,68,122,68,67,89,66,30,66,31,2,3,25,65,0,94,84,0,132,91,18,0,140,75,17,0,134,69,23,67,67,3,128,77,129,51,27,192,65,4,95,1,192,90,1,67,27,7,24,66,29,66,29,66,3,4,88,2,72,65,20,0,220,55,17,66,0,184,99,16,65,0,152,124,79,0,151,12,84,69,4,65,87,1,65,96,67,4,65,25,1,95,2,69,65,24,1,0,57,18,0,134,41,18,0,140,105,17,0,132,100,84,0,178,27,18,0,166,11,20,67,69,3,128,76,130,48,94,1,5,129,28,66,129,34,65,19,0,230,27,13,0,130,184,103,12,0,133,241,99,11,0,141,161,57,12,0,18,72,1,82,67,65,70,23,2,6,66,128,65,65,30,1,87,66,27,0,137,83,25,0,25,18,0,179,9,17,0,147,4,82,0,178,4,84,67,70,4,20,65,9,65,44};
// clang-format off

MicroAnimation animation(animationData, SSD1306_Display(display), 0, 0);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR);
}

void loop() {
  display.clearDisplay();
  unsigned long bestFrameTime = 0;
  unsigned long worstFrameTime = 0;
  unsigned long start = millis();
  for (unsigned int frame=0; frame<animation.getFrameCount(); frame++) {
    unsigned long frameStart = millis();

    animation.drawFrame(frame);

    // uncomment to instead test with animations directly from https://animator.wokwi.com/ (not incuded here)
    // display.clearDisplay();
    // display.drawBitmap(0, 0, frames[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    // display.display();

    unsigned long frameTime = millis() - frameStart;
    if (frameTime > worstFrameTime) {
      worstFrameTime = frameTime;
    }
    if (frameTime < bestFrameTime || bestFrameTime == 0) {
      bestFrameTime = frameTime;
    }
  }
  unsigned long end = millis();
  float fps = 1000.0 / ((end - start) / (float)animation.getFrameCount());
  unsigned int worstFps = 1000 / worstFrameTime;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("FPS:   ");
  display.println(fps);
  display.print("Best:  ");
  display.print(bestFrameTime);
  display.println("ms");
  display.print("Worst: ");
  display.print(worstFrameTime);
  display.print("ms (");
  display.print(worstFps);
  display.println(" FPS)");
  display.display();
  delay(5000);
}