#include <main.h>
#include <keycodes.h>

#define JITTER_THRESHOLD 250
#define HOLD_KEY_MS 100

#define BUF_LENGTH 8

const int num = 4;
const int pins[num] = {2, 3, 4, 5};
const bool keycodes_ctrl[num] = {false, false, false, false};
const bool keycodes_alt[num] = {false, false, false, true};
const bool keycodes_shift[num] = {false, false, false, true};
const uint8_t keycodes[num] = {KEY_G, KEY_H, KEY_Y, KEY_D};
long last_change[num];
bool cur_state[num];

void setup()
{
  // init serial communication with atmega16u2
  Serial.begin(9600);
  delay(100);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

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
      sendKeycode(keycodes[i], keycodes_ctrl[i], keycodes_alt[i], keycodes_shift[i]);
    }
  }
}

void sendKeycode(uint8_t keycode, bool ctrl, bool alt, bool shift)
{
  uint8_t buf[8]; /* Keyboard report buffer */
  for (byte i = 0; i < BUF_LENGTH; i++)
    buf[i] = 0;

  if (ctrl)
    buf[0] = KEY_LEFTCTRL;
  if (shift)
    buf[1] = KEY_LEFTSHIFT;
  if (alt)
    buf[2] = KEY_LEFTALT;
  buf[3] = keycode;

  sendAndRelease(buf);
}

void sendAndRelease(uint8_t *buf)
{
  Serial.write(buf, 8); // Send keypress
  for (int i = 0; i < BUF_LENGTH; i++)
    buf[i] = 0;
  delay(HOLD_KEY_MS);
  Serial.write(buf, 8); // Release key
}