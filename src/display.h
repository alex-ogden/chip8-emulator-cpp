#pragma once

#include <array>
#include <cstdint>

class Memory;

class Display {
public:
  static const uint8_t SCREEN_WIDTH = 64;
  static const uint8_t SCREEN_HEIGHT = 32;

  Display();
  void reset();
  void clear();
  void drawSprite(uint8_t x, uint8_t y, uint8_t height, Memory &memory, uint16_t i, uint8_t &vf);
  const std::array<std::array<bool, SCREEN_WIDTH>, SCREEN_HEIGHT> &getDisplay() const;

private:
  std::array<std::array<bool, SCREEN_WIDTH>, SCREEN_HEIGHT> display;
};