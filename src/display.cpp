#include "display.h"

#include "memory.h"

Display::Display() { reset(); }

void Display::reset() { clear(); }

void Display::clear() {
  for (auto &row : display)
    row.fill(false);
}

void Display::drawSprite(uint8_t x, uint8_t y, uint8_t height, Memory &memory, uint16_t i, uint8_t &vf) {
  vf = 0;

  for (uint8_t row = 0; row < height; ++row) {
    uint8_t spriteByte = memory.read(i + row);
    uint8_t yCoord = (y + row) % SCREEN_HEIGHT;

    for (uint8_t bit = 0; bit < 8; ++bit) {
      uint8_t xCoord = (x + bit) % SCREEN_WIDTH;
      uint8_t spritePixel = (spriteByte >> (7 - bit)) & 1;
      bool &pixelRef = display[yCoord][xCoord];

      if (spritePixel && pixelRef)
        vf = 1;
      pixelRef ^= spritePixel; // Toggle pixel
    }
  }
}

const std::array<std::array<bool, Display::SCREEN_WIDTH>, Display::SCREEN_HEIGHT> &Display::getDisplay() const {
  return display;
}