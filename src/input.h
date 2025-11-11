#pragma once

#include <array>
#include <cstdint>

class Input {
public:
  static const uint8_t NUM_KEYS = 16;

  Input();
  void reset();
  void setKey(uint8_t key, bool pressed);
  bool isKeyPressed(uint8_t key) const;

private:
  std::array<bool, NUM_KEYS> keys;
};