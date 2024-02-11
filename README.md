# MicroAnimation, an Arduino Library to play compressed animated icons on small devices

[![](https://img.shields.io/badge/Available_in_the_Arduino_Library_Manager-2ea44f)](https://www.arduino.cc/reference/en/libraries/microanimation/)

Plays compressed monochrome animations created by compressed by the [MicroAnimationConverter](https://bertbaron.github.io/MicroAnimationConverter/) on Adafruit displays.

Originally added to be able to play animated Icons from https://animator.wokwi.com/ on small devices like the Arduino Micro. Those animations can consume quite some program memory. They are however very compressible with simple Run Length Encoding in combination with Delta Compression.

As a bonus this library provides an easy API to deal with animations in different use cases. 

## Performance

Run Length Encoding is very simple and fast to decompres, and it doesn't require RAM for lookup tables or extra buffers. Often, especially when the animation is very well compressible, the decompression is faster than drawing the uncompressed bitmap because fewer operations need to be sent to the display, making use of drawFastHLine and skipping the parts that don't change from frame to frame. In some cases it might be somewhat slower though.

The [MicroAnimationConverter](https://bertbaron.github.io/MicroAnimationConverter/) will try to find the best compression method for each frame. In case it doesn't succeed in compressing a frame, it will just store the uncompressed bitmap, resulting in only marginal overhead in terms of memory and performance.

## Usage

Please see the examples for more information. All examples can be tested with the [MicroAnimationExample project on the Wokwi simulator](https://wokwi.com/projects/386250018661621761).

## Supported Displays

Currently only tested with the SSD1306. Feature/pull requests for other displays are welcome. The same goes for other types of displays.