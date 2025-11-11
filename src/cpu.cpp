#include "cpu.h"

#include "display.h"
#include "input.h"
#include "memory.h"

#include <SDL2/SDL.h>
#include <random>
#include <print>

CPU::CPU(Memory &memory, Display &display, Input &input)
    : memory(memory), display(display), input(input) {
  reset();
}

void CPU::reset() {
  v.fill(0);
  stack.fill(0);
  pc = 0x200;
  i = 0;
  sp = 0;
  delayTimer = 0;
  soundTimer = 0;
}

void CPU::emulateCycle() {
  uint16_t opcode = (static_cast<uint16_t>(memory.read(pc)) << 8) |
                    static_cast<uint16_t>(memory.read(pc + 1));

  // Increment the PC by 2
  pc += 2;

  executeInstruction(opcode);

  // Decrement timers at 60Hz
  static uint32_t lastTimerUpdate = 0;
  uint32_t currentTime = SDL_GetTicks();
  if (currentTime - lastTimerUpdate >= 1000 / 60) {
    if (delayTimer > 0)
      delayTimer--;
    if (soundTimer > 0)
      soundTimer--;
    lastTimerUpdate = currentTime;
  }
}

uint8_t CPU::randomByte() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<int> dist(0, 255);

  return static_cast<uint8_t>(dist(gen));
}

void CPU::executeInstruction(uint16_t opcode) {
  // Extract nibbles from opcode
  uint16_t nnn = opcode & 0x0FFF;
  uint8_t kk = opcode & 0x00FF;
  uint8_t n = opcode & 0x000F;
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;

  // Match the opcode on top nibble
  switch (opcode & 0xF000) {
  case 0x0000:
    switch (kk) {
    case 0xE0:
      cls();
      break;
    case 0xEE:
      ret();
      break;
    default:
      std::println(stderr, "Unknown 0x0000 opcode: {:#06X}", opcode);
      break;
    }
    break;
  case 0x1000:
    jp_addr(nnn);
    break;
  case 0x2000:
    call_addr(nnn);
    break;
  case 0x3000:
    se_vx_kk(x, kk);
    break;
  case 0x4000:
    sne_vx_kk(x, kk);
    break;
  case 0x5000:
    se_vx_vy(x, y);
    break;
  case 0x6000:
    ld_vx_kk(x, kk);
    break;
  case 0x7000:
    add_vx_kk(x, kk);
    break;
  case 0x8000:
    switch (n) {
    case 0x0:
      ld_vx_vy(x, y);
      break;
    case 0x1:
      or_vx_vy(x, y);
      break;
    case 0x2:
      and_vx_vy(x, y);
      break;
    case 0x3:
      xor_vx_vy(x, y);
      break;
    case 0x4:
      add_vx_vy(x, y);
      break;
    case 0x5:
      sub_vx_vy(x, y);
      break;
    case 0x6:
      shr_vx_vy(x, y);
      break;
    case 0x7:
      subn_vx_vy(x, y);
      break;
    case 0xE:
      shl_vx_vy(x, y);
      break;
    default:
      std::println(stderr, "Unknown 0x8000 opcode: {:#06X}", opcode);
      break;
    }
    break;
  case 0x9000:
    sne_vx_vy(x, y);
    break;
  case 0xA000:
    ld_i_addr(nnn);
    break;
  case 0xB000:
    jp_v0_addr(nnn);
    break;
  case 0xC000:
    rnd_vx_kk(x, kk);
    break;
  case 0xD000:
    drw_vx_vy_n(x, y, n);
    break;
  case 0xE000:
    switch (kk) {
    case 0x9E:
      skp_vx(x);
      break;
    case 0xA1:
      sknp_vx(x);
      break;
    default:
      std::println(stderr, "Unknown 0xE000 opcode: {:#06X}", opcode);
      break;
    }
    break;
  case 0xF000:
    switch (kk) {
    case 0x07:
      ld_vx_dt(x);
      break;
    case 0x0A:
      ld_vx_k(x);
      break;
    case 0x15:
      ld_dt_vx(x);
      break;
    case 0x18:
      ld_st_vx(x);
      break;
    case 0x1E:
      add_i_vx(x);
      break;
    case 0x29:
      ld_f_vx(x);
      break;
    case 0x33:
      ld_b_vx(x);
      break;
    case 0x55:
      ld_i_vx(x);
      break;
    case 0x65:
      ld_vx_i(x);
      break;
    default:
      std::println(stderr, "Unknown 0xF000 opcode: {:#06X}", opcode);
    }
    break;
  }
}

// Opcode instruction functions
void CPU::cls() { display.clear(); }
void CPU::ret() {
  sp -= 1;
  pc = stack[sp];
}
void CPU::jp_addr(uint16_t nnn) { pc = nnn; }
void CPU::call_addr(uint16_t nnn) {
  stack[sp] = pc;
  sp++;
  pc = nnn;
}
void CPU::se_vx_kk(uint8_t x, uint8_t kk) { pc += (v[x] == kk) * 2; }
void CPU::sne_vx_kk(uint8_t x, uint8_t kk) { pc += (v[x] != kk) * 2; }
void CPU::se_vx_vy(uint8_t x, uint8_t y) { pc += (v[x] == v[y]) * 2; }
void CPU::ld_vx_kk(uint8_t x, uint8_t kk) { v[x] = kk; }
void CPU::add_vx_kk(uint8_t x, uint8_t kk) { v[x] += kk; }
void CPU::ld_vx_vy(uint8_t x, uint8_t y) { v[x] = v[y]; }
void CPU::or_vx_vy(uint8_t x, uint8_t y) {
  v[x] |= v[y];
  v[0xF] = 0;
}
void CPU::and_vx_vy(uint8_t x, uint8_t y) {
  v[x] &= v[y];
  v[0xF] = 0;
}
void CPU::xor_vx_vy(uint8_t x, uint8_t y) {
  v[x] ^= v[y];
  v[0xF] = 0;
}
void CPU::add_vx_vy(uint8_t x, uint8_t y) {
  uint16_t result = v[x] + v[y];
  v[x] = result & 0xFF;
  v[0xF] = (result > 0xFF) ? 1 : 0;
}
void CPU::sub_vx_vy(uint8_t x, uint8_t y) {
  uint8_t old_vx = v[x];
  v[x] -= v[y];
  v[0xF] = (old_vx >= v[y]) ? 1 : 0;
}
void CPU::shr_vx_vy(uint8_t x, uint8_t y) {
  uint8_t flag = v[x] & 0x1;
  v[x] >>= 1;
  v[0xF] = flag;
}
void CPU::subn_vx_vy(uint8_t x, uint8_t y) {
  uint8_t old_vx = v[x];
  v[x] = v[y] - old_vx;
  v[0xF] = (v[y] >= old_vx) ? 1 : 0;
}
void CPU::shl_vx_vy(uint8_t x, uint8_t y) {
  uint8_t flag = (v[x] & 0x80) >> 7;
  v[x] <<= 1;
  v[0xF] = flag;
}
void CPU::sne_vx_vy(uint8_t x, uint8_t y) { pc += (v[x] != v[y]) * 2; }
void CPU::ld_i_addr(uint16_t nnn) { i = nnn; }
void CPU::jp_v0_addr(uint16_t nnn) {
  pc = nnn + (uint16_t)v[(nnn >> 8) & 0xF];
}
void CPU::rnd_vx_kk(uint8_t x, uint8_t kk) { v[x] = randomByte() & kk; }
void CPU::drw_vx_vy_n(uint8_t x, uint8_t y, uint8_t n) {
  display.drawSprite(v[x], v[y], n, memory, i, v[0xF]);
}
void CPU::skp_vx(uint8_t x) { pc += (v[x] < 16 && input.isKeyPressed(v[x])) * 2; }
void CPU::sknp_vx(uint8_t x) { pc += (v[x] >= 16 || !input.isKeyPressed(v[x])) * 2; }
void CPU::ld_vx_dt(uint8_t x) { v[x] = delayTimer; }
void CPU::ld_vx_k(uint8_t x) {
  bool keyPressed = false;

  for (uint8_t i = 0; i < 16; ++i) {
    if (input.isKeyPressed(i)) {
      v[x] = i;
      keyPressed = true;
      break;
    }
  }

  // If key not pressed, decrement PC to repeat instruction
  if (!keyPressed)
    pc -= 2;
}
void CPU::ld_dt_vx(uint8_t x) { delayTimer = v[x]; }
void CPU::ld_st_vx(uint8_t x) { soundTimer = v[x]; }
void CPU::add_i_vx(uint8_t x) { i += v[x]; }
void CPU::ld_f_vx(uint8_t x) { i = 0x50 + (uint16_t)(v[x] & 0x0F) * 5; }
void CPU::ld_b_vx(uint8_t x) {
  memory.write(i, v[x] / 100);
  memory.write(i + 1, (v[x] / 10) % 10);
  memory.write(i + 2, v[x] % 10);
}
void CPU::ld_i_vx(uint8_t x) {
  for (uint8_t reg = 0; reg <= x; ++reg)
    memory.write(i + reg, v[reg]);
  i += x + 1;
}
void CPU::ld_vx_i(uint8_t x) {
  for (uint8_t reg = 0; reg <= x; ++reg)
    v[reg] = memory.read(i + reg);
  i += x + 1;
}