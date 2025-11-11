#include "chip8.h"

#include "cpu.h"
#include "display.h"
#include "input.h"
#include "memory.h"

Chip8::Chip8() : memory(new Memory()), display(new Display()), input(new Input()), cpu(new CPU(*memory, *display, *input)) {
  reset();
}

Chip8::~Chip8() {
  delete memory;
  delete cpu;
  delete display;
  delete input;
}

void Chip8::reset() {
  memory->reset();
  cpu->reset();
  display->reset();
  input->reset();
}

bool Chip8::loadROM(const std::string &filename) {
  return memory->loadROM(filename);
}

void Chip8::emulateCycle() {
  cpu->emulateCycle();
}

void Chip8::setKey(uint8_t key, bool pressed) {
  input->setKey(key, pressed);
}

const std::array<std::array<bool, 64>, 32> &Chip8::getDisplay() const {
  return display->getDisplay();
}