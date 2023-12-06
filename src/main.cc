#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "SDL2/SDL.h"

#include "chip8.h"
#include "config.h"


const char keymap[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f
};


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("You must provide a file to load\n");
        return -1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "rb");

    if (!file) {
        printf("Failed to open file.\n");
        return -1;
    }

    // Get the required buffer size
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char buffer[size];
    int retcode = fread(buffer, size, 1, file);

    if (retcode != 1) {
        printf("Failed to read from file.\n");
        return -1;
    }

    Chip8 chip8;
    chip8.keyboard.set_keymap(keymap);
    chip8.load_code_from_buffer(buffer, size);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        EMULATOR_WINDOW_WIDTH,
        EMULATOR_WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while(true) {
        SDL_Event event;
        
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN: {
                    char key = event.key.keysym.sym;
                    char vkey = chip8.keyboard.map(key);
                    if(vkey != -1) { chip8.keyboard.key_down(vkey); }
                    break;
                }

                case SDL_KEYUP: {
                    char key = event.key.keysym.sym;
                    char vkey = chip8.keyboard.map(key);
                    if(vkey != -1) { chip8.keyboard.key_up(vkey); }
                    break;
                }

                case SDL_QUIT: {
                    goto EXIT;
                    break;
                }
            };
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for (int x=0; x<CHIP8_DISPLAY_WIDTH; x++) {
            for (int y=0; y<CHIP8_DISPLAY_HEIGHT; y++) {
                if (chip8.screen.is_set(x, y)) {
                    SDL_Rect rectangle;
                    rectangle.x = x * EMULATOR_WINDOW_SCALE;  // SDL_Rect.x --> width
                    rectangle.y = y * EMULATOR_WINDOW_SCALE;  // SDL_Rect.y --> height
                    rectangle.w = EMULATOR_WINDOW_SCALE;
                    rectangle.h = EMULATOR_WINDOW_SCALE;
                    SDL_RenderFillRect(renderer, &rectangle);
                }
            }
        }

        SDL_RenderPresent(renderer);

        if (chip8.registers.delay_timer > 0) {
            Sleep(1);
            chip8.registers.delay_timer -= 1;
        }

        if (chip8.registers.sound_timer > 0) {
            Beep(5000, 1 * chip8.registers.sound_timer);
            chip8.registers.sound_timer = 0;
        }

        unsigned short opcode = chip8.memory.get_word(chip8.registers.program_counter);
        chip8.registers.program_counter += 2;
        chip8.exec(opcode);
    }

EXIT:
    SDL_DestroyWindow(window);
    return 0;
}
