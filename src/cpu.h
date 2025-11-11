#pragma once

#include <array>
#include <cstdint>

class Memory;
class Display;
class Input;

class CPU {
public:
  CPU(Memory &memory, Display &display, Input &input);

  void reset();
  void emulateCycle();
  void executeInstruction(uint16_t opcode);

  // Register access
  uint16_t getPC() const { return pc; }
  void setPC(uint16_t value) { pc = value; }
  uint16_t getI() const { return i; }
  void setI(uint16_t value) { i = value; }
  uint8_t getSP() const { return sp; }
  void setSP(uint8_t value) { sp = value; }
  uint8_t getDelayTimer() const { return delayTimer; }
  void setDelayTimer(uint8_t value) { delayTimer = value; }
  uint8_t getSoundTimer() const { return soundTimer; }
  void setSoundTimer(uint8_t value) { soundTimer = value; }
  uint8_t getV(uint8_t index) const { return v[index]; }
  void setV(uint8_t index, uint8_t value) { v[index] = value; }
  uint16_t getStack(uint8_t index) const { return stack[index]; }
  void setStack(uint8_t index, uint16_t value) { stack[index] = value; }

private:
  Memory &memory;
  Display &display;
  Input &input;

  std::array<uint8_t, 16> v;     // V0 to VF registers
  std::array<uint16_t, 16> stack; // 16 16-bit stack entries
  uint16_t pc;                   // Program counter
  uint16_t i;                    // Index register
  uint8_t sp;                    // Stack pointer
  uint8_t delayTimer;            // Delay timer
  uint8_t soundTimer;            // Sound timer

  // Random byte generator
  uint8_t randomByte();

  // Instruction implementations
  void ret();
  void cls();
  void jp_addr(uint16_t nnn);
  void call_addr(uint16_t nnn);
  void se_vx_kk(uint8_t x, uint8_t kk);
  void sne_vx_kk(uint8_t x, uint8_t kk);
  void se_vx_vy(uint8_t x, uint8_t y);
  void ld_vx_kk(uint8_t x, uint8_t kk);
  void add_vx_kk(uint8_t x, uint8_t kk);
  void ld_vx_vy(uint8_t x, uint8_t y);
  void or_vx_vy(uint8_t x, uint8_t y);
  void and_vx_vy(uint8_t x, uint8_t y);
  void xor_vx_vy(uint8_t x, uint8_t y);
  void add_vx_vy(uint8_t x, uint8_t y);
  void sub_vx_vy(uint8_t x, uint8_t y);
  void shr_vx_vy(uint8_t x, uint8_t y);
  void subn_vx_vy(uint8_t x, uint8_t y);
  void shl_vx_vy(uint8_t x, uint8_t y);
  void sne_vx_vy(uint8_t x, uint8_t y);
  void ld_i_addr(uint16_t nnn);
  void jp_v0_addr(uint16_t nnn);
  void rnd_vx_kk(uint8_t x, uint8_t kk);
  void drw_vx_vy_n(uint8_t x, uint8_t y, uint8_t n);
  void skp_vx(uint8_t x);
  void sknp_vx(uint8_t x);
  void ld_vx_dt(uint8_t x);
  void ld_vx_k(uint8_t x);
  void ld_dt_vx(uint8_t x);
  void ld_st_vx(uint8_t x);
  void add_i_vx(uint8_t x);
  void ld_f_vx(uint8_t x);
  void ld_b_vx(uint8_t x);
  void ld_i_vx(uint8_t x);
  void ld_vx_i(uint8_t x);
};