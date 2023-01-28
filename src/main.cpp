#include <main.h>
#include <keycodes.h>

#define JITTER_THRESHOLD 250

const int num = 4;
const int pins[num] = {2, 3, 4, 5};
const uint8_t keycodes[num] = {KEY_G, KEY_H, KEY_Y, KEY_D};
long last_change[num];
bool cur_state[num];

void setup()
{
  // init serial communication with atmega16u2
  Serial.begin(9600);
  delay(100);

  // set mode, wait for caps to charge and save states
  for (int i = 0; i < num; i++)
  {
    pinMode(pins[i], INPUT_PULLUP);
    delay(10);
    last_change[i] = 0;
    cur_state[i] = digitalRead(pins[i]);
  }
}

void loop()
{
  // iterate over buttons
  for (int i = 0; i < num; i++)
  {
    // if button state has changed
    if (digitalRead(pins[i]) != cur_state[i])
    {
      // jitter check
      if (millis() - last_change[i] < JITTER_THRESHOLD)
      {
        last_change[i] = millis();
        continue;
      }

      // save info about change, send corresponding key
      last_change[i] = millis();
      cur_state[i] = digitalRead(pins[i]);
      sendKeycode(keycodes[i]);
    }
  }
}

void sendKey(char key)
{
  uint8_t buf[8] = {0}; /* Keyboard report buffer */
  if ((key >= 'a') && (key <= 'z'))
  {
    buf[2] = key - 'a' + 4;
  }
  else if ((key >= 'A') && (key <= 'Z'))
  {
    buf[0] = KEY_LEFTSHIFT; /* Caps */
    buf[2] = key - 'A' + 4;
  }
  sendAndRelease(buf);
}

void sendKeycode(uint8_t keycode)
{
  uint8_t buf[8] = {0}; /* Keyboard report buffer */
  buf[2] = keycode;
  sendAndRelease(buf);
}

void sendAndRelease(uint8_t *buf)
{
  Serial.write(buf, 8); // Send keypress
  buf[0] = 0;
  buf[2] = 0;
  delay(500);
  Serial.write(buf, 8); // Release key
}