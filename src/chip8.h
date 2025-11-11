#pragma once

#include <string>

class Memory;
class CPU;
class Display;
class Input;

class Chip8 {
public:
  static const uint8_t SCREEN_WIDTH = 64;  // Window width
  static const uint8_t SCREEN_HEIGHT = 32; // Window height
  static const uint8_t PIXEL_SIZE = 10;    // Actual pixels per CHIP8-pixel

  Chip8();
  ~Chip8();
  void reset();
  bool loadROM(const std::string &filename);
  void emulateCycle();
  void setKey(uint8_t key, bool pressed);
  const std::array<std::array<bool, 64>, 32> &getDisplay() const;

private:
  Memory *memory;
  Display *display;
  Input *input;
  CPU *cpu;
};
