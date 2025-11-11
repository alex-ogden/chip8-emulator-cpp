#include "chip8.h"

#include <SDL2/SDL.h>
#include <chrono>
#include <filesystem>
#include <map>
#include <string>
#include <thread>

int main(int argc, char *argv[]) {
  Chip8 chip8;

  if (argc < 2) {
    std::println(stderr, "Usage: {} <path_to_rom>", argv[0]);
    return 1;
  }
  // Current running executable from build/ directory
  std::string filename = argv[1];

  // Ensure ROM file exists
  if (std::filesystem::exists(filename)) {
    // Load ROM into CHIP8 memory if it exists
    if (!chip8.loadROM(filename)) {
      std::println(stderr, "Failed to load ROM: {}", filename);
    }
  } else {
    std::println(stderr, "File {} doesn't exist", filename);
    return 1;
  }

  // Initialise SDL
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::println(stderr, "SDL Init Error: {}", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Chip8::SCREEN_WIDTH * Chip8::PIXEL_SIZE,
                            Chip8::SCREEN_HEIGHT * Chip8::PIXEL_SIZE, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Create the key map
  std::map<SDL_Keycode, uint8_t> keymap;

  keymap.insert({SDLK_1, 0x1});
  keymap.insert({SDLK_2, 0x2});
  keymap.insert({SDLK_3, 0x3});
  keymap.insert({SDLK_4, 0xC});
  keymap.insert({SDLK_q, 0x4});
  keymap.insert({SDLK_w, 0x5});
  keymap.insert({SDLK_e, 0x6});
  keymap.insert({SDLK_r, 0xD});
  keymap.insert({SDLK_a, 0x7});
  keymap.insert({SDLK_s, 0x8});
  keymap.insert({SDLK_d, 0x9});
  keymap.insert({SDLK_f, 0xE});
  keymap.insert({SDLK_z, 0xA});
  keymap.insert({SDLK_x, 0x0});
  keymap.insert({SDLK_c, 0xB});
  keymap.insert({SDLK_v, 0xF});

  SDL_Event event;
  bool running = true;

  // Main game loop
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_KEYDOWN: {
        const auto key = event.key.keysym.sym;
        if (key == SDLK_ESCAPE)
          running = false;
        if (keymap.find(key) != keymap.end())
          chip8.setKey(keymap[key], true);
      } break;
      case SDL_KEYUP: {
        const auto key = event.key.keysym.sym;
        if (keymap.find(key) != keymap.end())
          chip8.setKey(keymap[key], false);
      } break;
      default:
        break;
      }
    }

    // Emulate a cycle
    chip8.emulateCycle();

    // Update display at 60Hz
    static uint32_t last_draw = 0;
    uint32_t now = SDL_GetTicks();
    if (now - last_draw >= 16) {  // ~60Hz
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      for (int y = 0; y < Chip8::SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < Chip8::SCREEN_WIDTH; ++x) {
          if (chip8.getDisplay()[y][x]) {
            SDL_Rect rect = {x * Chip8::PIXEL_SIZE, y * Chip8::PIXEL_SIZE, Chip8::PIXEL_SIZE,
                             Chip8::PIXEL_SIZE};
            SDL_RenderFillRect(renderer, &rect);
          }
        }
      }

      SDL_RenderPresent(renderer);
      last_draw = now;
    }

    // Small delay to not hog CPU
    std::this_thread::sleep_for(std::chrono::microseconds(100));
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
