#include <Arduino.h>
void sendKeycode(uint8_t keycode, bool ctrl, bool alt, bool shift);
void sendAndRelease(uint8_t *buf);