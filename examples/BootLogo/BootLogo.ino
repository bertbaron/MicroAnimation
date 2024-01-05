#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <compressed_animation.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// clang-format off
const byte PROGMEM animationData[] = {28,32,32,59,0,121,0,124,0,127,0,130,0,148,0,157,0,162,0,170,0,177,0,182,0,187,0,190,0,193,0,196,0,199,0,202,0,205,0,208,0,226,0,244,0,4,1,16,1,26,1,36,1,45,1,53,1,56,1,65,129,91,1,30,1,30,2,30,1,30,1,30,2,30,1,30,2,29,2,30,1,30,2,29,2,15,1,14,1,17,2,11,2,18,2,9,2,20,2,8,1,22,2,6,2,23,2,5,1,25,2,3,1,27,2,1,2,28,3,129,49,129,136,0,129,136,0,129,136,0,129,132,68,15,82,13,84,11,86,10,87,8,89,7,90,1,193,117,129,133,17,1,126,1,95,130,16,129,132,20,131,108,129,129,90,66,129,59,132,105,130,129,91,192,125,133,40,129,130,25,133,103,129,129,122,134,6,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,136,0,65,132,68,1,32,2,31,2,31,2,31,2,31,2,31,2,129,116,129,133,49,65,30,66,30,65,30,65,27,66,1,66,28,67,129,49,129,131,117,65,30,66,29,66,30,65,30,66,30,65,130,109,129,130,119,66,30,65,30,66,29,66,132,41,129,130,25,65,31,65,30,65,133,39,129,129,122,65,31,65,62,65,133,38,129,129,90,66,128,125,30,197,9,130,129,91,195,54,66,130,109,129,136,0,129,136,0};
// clang-format on

Animation checkmark(animationData, &display, 48, 0);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  checkmark.play();
}

void loop() {
  delay(100);
}