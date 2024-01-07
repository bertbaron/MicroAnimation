#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <MicroAnimation.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RST_PIN -1
#define SCREEN_I2C_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST_PIN);

// clang-format off
const byte PROGMEM animationData[] = {28,32,32,59,0,163,0,166,0,169,0,172,0,178,0,186,0,200,0,214,0,234,0,245,0,6,1,13,1,22,1,25,1,28,1,31,1,45,1,65,1,87,1,113,1,129,1,149,1,167,1,179,1,185,1,191,1,194,1,65,129,77,2,30,2,30,2,30,2,29,3,29,4,28,1,1,2,24,1,3,1,2,1,4,2,17,2,3,1,2,1,4,2,17,3,1,2,2,1,4,3,15,1,2,1,1,1,3,1,3,2,1,1,11,5,2,3,3,2,2,2,1,1,2,5,11,3,3,2,2,1,2,4,16,2,4,1,2,1,3,2,17,1,5,1,1,2,3,1,24,1,1,1,29,3,29,3,29,2,31,1,129,78,129,136,0,129,136,0,129,136,0,129,132,34,1,195,93,129,132,7,65,27,3,195,90,129,131,40,4,91,1,95,1,94,3,93,3,195,87,129,131,72,4,92,3,94,2,94,2,94,2,195,53,129,131,44,3,93,3,92,4,92,4,92,4,92,4,91,6,90,6,194,112,130,130,46,94,2,94,2,94,2,196,114,130,131,52,91,5,91,5,91,4,92,4,92,4,93,3,195,13,129,133,16,7,89,130,80,130,132,57,90,3,93,4,195,9,65,136,0,65,136,0,65,136,0,65,131,40,1,30,2,30,1,30,1,27,5,131,89,129,131,104,66,1,66,28,65,1,65,29,67,29,67,30,66,30,65,130,117,129,129,77,66,30,66,30,66,30,65,30,66,30,66,30,65,31,65,31,65,132,51,129,130,46,65,31,65,31,66,30,66,31,65,31,65,31,65,31,65,31,66,30,66,31,65,131,15,129,133,16,65,31,65,31,67,29,67,29,66,31,65,129,78,129,131,84,65,31,65,30,66,30,65,31,65,31,65,30,66,30,65,130,77,129,131,52,66,31,65,31,66,31,65,29,65,1,65,31,66,131,40,129,132,57,65,30,66,29,66,30,65,130,104,129,132,58,67,131,67,129,132,61,65,131,66,129,136,0,129,136,0};
// clang-format on

MicroAnimation animations[] = {
    MicroAnimation(animationData, &display, 0, 0),
    MicroAnimation(animationData, &display, 48, 0),
    MicroAnimation(animationData, &display, 96, 0),
};

byte buttonPins[] = {8, 9, 10};
bool buttonStates[] = {false, false, false};

int buttonPressed();

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR);

  for (byte i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Run animations at different rates
  animations[0].setFrameDelay(30);
  animations[1].setFrameDelay(60);
  animations[2].setFrameDelay(90);

  display.clearDisplay();
  for (byte i = 0; i < 3; i++) {
    animations[i].start();
  }
}

void loop() {
  for (byte i = 0; i < 3; i++) {
    animations[i].update();
  }

  int button = buttonPressed();
  if (button != -1) {
    if (animations[button].isRunning()) {
      animations[button].stop(true); // stop after finishing current loop cycle
    } else {
      animations[button].start(true); // start and loop
    }
  }

  delay(1);
}

// Returns index of button when state changes from HIGH to LOW, -1 if no button is pressed
int buttonPressed() {
  for (byte i = 0; i < 3; i++) {
    bool state = digitalRead(buttonPins[i]);
    if (state != buttonStates[i]) {
      buttonStates[i] = state;
      if (state == LOW) {
        return i;
      }
    }
  }
  return -1;
}