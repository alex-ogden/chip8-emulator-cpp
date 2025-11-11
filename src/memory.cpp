#include "memory.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <print>
#include <string>
#include <vector>

Memory::Memory() { reset(); }

void Memory::reset() {
  memory.fill(0);
  loadFontset();
}

bool Memory::loadROM(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  if (!file) {
    std::println(stderr, "Could not open file");
    return false;
  }
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<uint8_t> buffer(size);
  if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
    std::println("Could not read file into buffer vector");
    return false;
  }

  // Ensure the size of the buffer is not greater than the size of the memory
  if (buffer.size() <= memory.size() - ROM_START_ADDRESS) {
    for (size_t i = 0; i < buffer.size() && ROM_START_ADDRESS + i < memory.size(); ++i)
      memory[ROM_START_ADDRESS + i] = buffer[i];
  } else {
    std::println("ROM size ({}) is greater than available memory ({})",
                 buffer.size(), memory.size());
    return false;
  }

  // Return success
  return true;
}

void Memory::loadFontset() {
  std::array<uint8_t, 80> fonts = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80, // F
  };

  std::copy(fonts.begin(), fonts.end(),
            memory.begin() + FONT_START_ADDRESS);
}

uint8_t Memory::read(uint16_t address) const {
  return memory[address];
}

void Memory::write(uint16_t address, uint8_t value) {
  memory[address] = value;
}

const std::array<uint8_t, Memory::MEMORY_SIZE> &Memory::getMemory() const {
  return memory;
}