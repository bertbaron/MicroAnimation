#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <MicroAnimation.h>
#include <adapters/SSD1306_Display.h>

// Test it here (copy this code): https://wokwi.com/projects/386250018661621761

/*
 * More complex example showing how a menu with animated feedback can be implemented without blocking.
 * The blinking led shows that the loop is being continued. Processing an animation may affect the
 * regularity of the blinking depending on the hardware and the complexity of the animation.
 *
 * This also contains an example of using an onFinish callback handler.
 * 
 * Please note that the Wokwi simulator is not a good indication of the actual performance on real hardware.
 */

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RST_PIN -1
#define SCREEN_I2C_ADDR 0x3D
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST_PIN);

// With icons from https://icons8.com/

// clang-format off
const byte PROGMEM checkmarkData[] = {0,28,32,32,60,0,120,0,123,0,126,0,129,0,147,0,156,0,161,0,169,0,176,0,181,0,186,0,189,0,192,0,195,0,198,0,201,0,204,0,207,0,225,0,242,0,2,1,14,1,24,1,34,1,43,1,51,1,54,1,65,129,91,1,30,1,30,2,30,1,30,1,30,2,30,1,30,2,29,2,30,1,30,2,29,2,15,1,14,1,17,2,11,2,18,2,9,2,20,2,8,1,22,2,6,2,23,2,5,1,24,0,49,25,0,27,28,3,129,49,129,136,0,129,136,0,129,136,0,129,132,68,15,82,13,84,11,86,10,87,8,89,7,90,1,193,117,129,133,17,1,126,1,95,130,16,129,132,20,131,108,129,129,90,66,129,59,132,105,130,129,91,192,125,133,40,129,130,25,133,103,129,129,122,134,6,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,132,68,1,32,2,31,2,31,2,31,2,31,2,31,2,129,116,129,133,49,65,30,66,30,65,30,65,25,0,27,28,67,129,49,129,131,117,65,30,66,29,66,30,65,30,66,30,65,130,109,129,130,119,66,30,65,30,66,29,66,132,41,129,130,25,65,31,65,30,65,133,39,129,129,122,65,31,65,62,65,133,38,129,129,90,66,128,125,30,197,9,130,129,91,195,54,66,130,109,129,136,0,129,136,0};
const byte PROGMEM crossData[] = {0,28,32,32,60,0,152,0,155,0,158,0,161,0,170,0,186,0,202,0,220,0,231,0,236,0,241,0,244,0,247,0,250,0,253,0,0,1,3,1,6,1,16,1,32,1,48,1,71,1,94,1,110,1,126,1,136,1,139,1,65,129,4,1,22,1,9,1,20,1,11,1,18,1,13,1,16,1,15,1,14,1,17,1,12,1,19,1,10,1,21,1,8,1,23,1,6,1,24,0,33,24,0,9,29,2,30,2,26,0,9,26,0,33,25,1,6,1,23,1,8,1,21,1,10,1,19,1,12,1,17,1,14,1,15,1,16,1,13,1,18,1,11,1,20,1,9,1,22,1,129,4,129,136,0,129,136,0,129,136,0,130,129,27,74,21,76,19,198,39,129,129,103,17,80,15,82,13,84,11,86,9,88,7,196,109,129,131,45,5,92,3,94,1,96,30,67,28,69,26,195,20,130,129,120,194,123,24,73,22,75,20,77,18,79,16,81,14,193,90,130,131,50,195,7,12,85,10,87,129,5,130,132,108,195,20,130,134,38,193,90,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,129,4,1,32,1,32,1,134,57,129,129,103,65,32,65,32,65,32,65,32,65,32,65,132,115,129,131,45,65,32,65,32,65,32,65,32,65,32,65,131,45,129,129,27,65,30,65,30,65,131,25,65,32,65,32,65,32,65,32,65,32,65,129,103,129,129,120,65,30,65,30,65,30,65,30,65,30,65,131,37,65,32,65,32,65,129,4,129,131,50,65,30,65,30,65,30,66,29,65,30,65,131,50,129,132,108,65,30,65,30,65,30,65,30,65,30,65,129,120,129,134,38,65,30,65,30,65,129,27,129,136,0,129,136,0};
const byte PROGMEM refreshData[] = {0,28,32,32,60,0,158,0,161,0,164,0,167,0,197,0,238,0,17,1,54,1,95,1,144,1,155,1,182,1,213,1,243,1,14,2,43,2,75,2,101,2,128,2,147,2,174,2,203,2,242,2,29,3,60,3,92,3,95,3,65,77,6,23,9,22,1,30,1,30,2,30,1,16,1,14,1,15,3,13,1,14,5,12,1,11,0,146,224,129,128,130,19,10,1,16,1,14,1,16,1,14,1,16,1,14,1,16,1,14,1,16,1,14,1,16,1,14,1,16,1,14,1,15,0,160,131,17,12,1,11,0,105,13,1,12,5,14,1,13,3,15,1,14,1,16,1,30,2,30,1,29,2,22,9,23,6,77,129,136,0,129,136,0,129,136,0,129,129,119,66,29,68,27,70,25,66,0,19,30,1,194,8,3,92,66,0,19,28,1,2,30,65,30,65,129,119,129,128,82,24,72,23,192,110,1,98,65,26,0,107,25,3,92,4,68,11,193,124,4,68,13,82,1,89,66,65,94,69,30,16,192,117,1,95,1,192,82,129,128,81,25,193,13,66,28,70,25,66,0,19,23,4,67,12,194,60,4,68,13,75,66,0,19,24,71,29,65,129,37,1,192,81,129,128,80,26,70,25,193,12,1,68,13,77,1,6,76,77,0,37,130,73,0,41,24,2,6,77,76,2,68,14,193,22,2,95,1,192,80,130,128,106,68,27,192,79,65,61,1,66,14,78,2,67,13,78,1,4,77,194,62,1,4,78,77,1,67,15,78,1,66,15,111,65,128,104,2,94,128,80,129,128,105,66,93,2,12,65,32,65,31,66,30,1,95,1,95,1,65,14,79,2,65,14,194,63,1,65,16,78,1,65,16,80,16,80,16,80,65,31,65,32,65,76,2,93,2,128,105,130,128,116,2,96,197,96,20,2,192,116,129,128,81,66,29,70,31,66,96,80,16,80,17,196,32,15,80,17,79,18,2,33,68,29,66,128,81,129,128,77,70,25,74,129,17,17,79,17,79,17,79,17,194,32,15,81,15,81,15,81,15,193,9,68,27,68,128,77,129,128,106,76,19,66,129,124,17,79,17,79,17,79,17,96,15,81,15,81,15,81,15,193,110,66,29,66,128,106,129,129,10,11,83,65,30,66,30,65,31,65,129,16,129,111,192,113,65,31,65,30,66,30,65,129,40,129,129,21,1,193,2,15,1,95,1,95,1,95,1,129,112,65,95,1,95,1,95,1,95,192,114,11,193,11,129,129,55,16,80,16,81,15,193,32,65,31,65,31,65,48,65,31,65,31,65,129,46,17,79,16,81,15,83,65,129,21,129,128,116,21,77,18,194,16,65,31,65,31,65,14,65,31,65,31,65,130,33,12,85,2,192,116,129,128,81,25,72,23,193,79,65,31,65,31,65,129,78,65,31,65,31,65,129,100,2,95,2,192,81,129,128,80,26,70,25,193,47,65,131,14,65,129,70,2,95,1,192,80,129,128,79,27,193,14,65,30,66,30,67,28,68,130,76,68,28,67,30,66,30,65,129,40,1,192,79,129,128,79,66,31,65,129,37,68,27,70,25,0,37,130,73,0,41,26,70,27,68,129,37,71,25,68,128,77,129,128,81,65,31,65,129,5,66,28,70,25,66,0,19,24,3,66,13,194,62,2,67,14,75,66,0,19,24,71,29,65,129,5,72,24,69,128,77,129,128,82,65,31,65,129,4,1,93,1,94,1,0,107,75,77,65,6,66,130,70,66,6,65,23,1,0,107,77,76,1,69,14,80,16,192,117,73,23,70,128,77,129,129,119,66,33,13,80,68,76,78,65,30,65,7,65,130,6,65,7,65,30,66,24,1,3,29,1,98,65,129,119,129,129,119,1,94,1,94,1,94,1,69,66,75,84,66,130,7,65,30,1,2,5,13,80,66,78,81,15,80,16,193,104,129,136,0,129,136,0};
// clang-format on

MicroAnimation checkmark(checkmarkData, SSD1306_Display(display), 48, 16);
MicroAnimation cross(crossData, SSD1306_Display(display), 48, 16);
MicroAnimation refresh(refreshData, SSD1306_Display(display), 48, 16);

byte buttonPins[] = {5, 4, 3};
bool buttonStates[] = {false, false, false};

// We just play one animation at a time, so we use a pointer to the current animation to avoid code
// duplication. Not that update() only needs to be called on a running animation
MicroAnimation* currentAnimation = NULL;
bool menuOn = false;

unsigned long lastBlinkTime = 0;
int blinkDelay = 400;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR);
  display.setTextColor(SSD1306_WHITE);

  for (byte i = 0; i < 3; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(LED_BUILTIN, OUTPUT);

  mainScreen();
}

void loop() {
  // stuff that should always be done
  processImportantStuff();

  if (currentAnimation != NULL) {
    if (currentAnimation->update()) {
      return; // skip other stuff during playback
    }
    if (currentAnimation->isFinished()) {
      // stuff that should happen when the animation finishes
      currentAnimation = NULL;
      updateScreen();
    }
  }
  
  // stuff that should not be done while animation is playing
  processMenu();
}

// Blink the built-in led to show the 'important' stuff just continues'
void processImportantStuff() {
  unsigned long now = millis();
  if (now - lastBlinkTime > blinkDelay) {
      digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == HIGH ? LOW : HIGH);
      lastBlinkTime = now;
  } 
}

void processMenu() {
  int button = buttonPressed();
  if (button < 0) {
    return;
  }

  if (!menuOn) {
    if (button == 1) {
      menuOn = true;
      updateScreen();
    }
  } else {
    display.clearDisplay();
    switch (button) {
      case 0:
        // Just because we can we adjust the blink delay AFTER playing the animation
        checkmark.onFinish([]() {
          blinkDelay = 1200 - blinkDelay;
        });
        currentAnimation = &checkmark;
        menuOn = false;
        break;
      case 1:
        currentAnimation = &cross;
        menuOn = false;
        break;
      case 2:
        currentAnimation = &refresh;
        break;
    }
    if (currentAnimation != NULL) {
      currentAnimation->start();
    }
  }
}

void updateScreen() {
  if (menuOn) {
    menuScreen();
  } else {
    mainScreen();
  }
}

void mainScreen() {
  display.clearDisplay();
  display.setCursor(50,56);
  display.println("menu");
  display.display();
}

void menuScreen() {
  display.clearDisplay();
  display.setCursor(10, 23);
  display.println("Adjust blink rate?");
  display.setCursor(0,56);
  display.println("yes      no     maybe");
  display.display();
}

int lastPressed = 0;
// Returns index of button when pressed, -1 if no button is pressed
int buttonPressed() {
  unsigned long now = millis();
  if (now - lastPressed < 500) {
    return -1; // handle debounce and accidental repeats
  }
  for (byte i = 0; i < 3; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      lastPressed = now;
      return i;
    }
  }
  return -1;
}
