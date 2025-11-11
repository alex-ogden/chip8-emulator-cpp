#pragma once

#include <array>
#include <cstdint>
#include <string>

class Memory {
public:
  static const uint16_t MEMORY_SIZE = 4096;
  static const uint16_t ROM_START_ADDRESS = 0x200;
  static const uint16_t FONT_START_ADDRESS = 0x50;

  Memory();
  void reset();
  bool loadROM(const std::string &filename);
  void loadFontset();
  uint8_t read(uint16_t address) const;
  void write(uint16_t address, uint8_t value);
  const std::array<uint8_t, MEMORY_SIZE> &getMemory() const;

private:
  std::array<uint8_t, MEMORY_SIZE> memory;
};