#include "input.h"

Input::Input() { reset(); }

void Input::reset() { keys.fill(false); }

void Input::setKey(uint8_t key, bool pressed) {
  if (key < NUM_KEYS) {
    keys[key] = pressed;
  }
}

bool Input::isKeyPressed(uint8_t key) const {
  if (key < NUM_KEYS) {
    return keys[key];
  }
  return false;
}