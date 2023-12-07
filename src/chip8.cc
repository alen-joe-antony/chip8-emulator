#include <cassert>
#include <ctime>
#include <cstring>
#include <cstddef>
#include <cstdlib>

#include "SDL2/SDL.h"

#include "chip8.h"
#include "config.h"

const unsigned char default_charset[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};


Chip8::Chip8(void) {
    memcpy(&memory.memory, default_charset, sizeof(default_charset));
}


void Chip8::load_code_from_buffer(const unsigned char *buffer, size_t size) {
    assert(size + CHIP8_PROGRAM_LOAD_ADDR < CHIP8_MEMORY_SIZE);
    memcpy(&memory.memory[CHIP8_PROGRAM_LOAD_ADDR], buffer, size);
    registers.program_counter = CHIP8_PROGRAM_LOAD_ADDR;
}


char Chip8::wait_for_key_press(void) {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type != SDL_KEYDOWN) {
            continue;
        }
        return event.key.keysym.sym;
    }
    return -1;
}


void Chip8::exec_extended_8_series(unsigned short opcode) {
    unsigned char x = (opcode >> 8) & 0x000F;
    unsigned char y = (opcode >> 4) & 0x000F;

    switch(opcode & 0x000F) {
        // 8xy0 - LD Vx, Vy
        // Stores the value of register Vy in register Vx.
        case 0x00: {
            registers.reg_v[x] = registers.reg_v[y];
            break;
        }
        
        // 8xy1 - OR Vx, Vy
        // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
        case 0x01: {
            registers.reg_v[x] |= registers.reg_v[y];
            break;
        }

        // 8xy2 - AND Vx, Vy
        // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
        case 0x02: {
            registers.reg_v[x] &= registers.reg_v[y];
            break;
        }

        // 8xy3 - XOR Vx, Vy
        // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
        case 0x03: {
            registers.reg_v[x] ^= registers.reg_v[y];
            break;
        }

        // 8xy4 - ADD Vx, Vy
        // The values of Vx and Vy are added together. 
        // If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. 
        // Only the lowest 8 bits of the result are kept, and stored in Vx.
        case 0x04: {
            unsigned short sum = registers.reg_v[x] + registers.reg_v[y];
            registers.reg_v[0x0F] = sum > 0xFF;
            registers.reg_v[x] = sum;
            break;
        }
            
        // 8xy5 - SUB Vx, Vy
        // If Vx > Vy, then VF is set to 1, otherwise 0. 
        // Then Vy is subtracted from Vx, and the results stored in Vx.
        case 0x05: {
            registers.reg_v[0x0F] = registers.reg_v[x] > registers.reg_v[y];
            registers.reg_v[x] -= registers.reg_v[y];
            break;
        }

        // 8xy6 - SHR Vx {, Vy}
        // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
        // Then Vx is divided by 2.
        case 0x06: {
            registers.reg_v[0x0F] = registers.reg_v[x] & 0x01;
            registers.reg_v[x] >>= 1;
            break;
        }

        // 8xy7 - SUBN Vx, Vy
        // If Vy > Vx, then VF is set to 1, otherwise 0. 
        // Then Vx is subtracted from Vy, and the results stored in Vx.
        case 0x07: {
            registers.reg_v[0x0F] = registers.reg_v[y] > registers.reg_v[x];
            registers.reg_v[x] = registers.reg_v[y] - registers.reg_v[x];
            break;
        }

        // 8xyE - SHL Vx {, Vy}
        // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
        // Then Vx is multiplied by 2.
        case 0x0E: {
            registers.reg_v[0x0F] = registers.reg_v[x] & 0b10000000;
            registers.reg_v[x] <<= 1;
            break;
        }
    };
}


void Chip8::exec_extended_E_series(unsigned short opcode) {
    unsigned char x = (opcode >> 8) & 0x000F;
    switch (opcode & 0x00FF) {
        // Ex9E - SKP Vx
        // If key with the value of Vx is pressed, PC is increased by 2.
        case 0x9E: {
            registers.program_counter += 2 * keyboard.is_key_down(registers.reg_v[x]);
            break;
        }

        // ExA1 - SKNP Vx
        // If key with the value of Vx is not pressed, PC is increased by 2.
        case 0xA1: {
            registers.program_counter += 2 * !keyboard.is_key_down(registers.reg_v[x]);
            break;
        }
    };
}


void Chip8::exec_extended_F_series(unsigned short opcode){
    unsigned char x = (opcode >> 8) & 0x000F;
    switch (opcode & 0x00FF) {
        // Fx07 - LD Vx, DT
        // The value of delay timer is stored into Vx.
        case 0x07: {
            registers.reg_v[x] = registers.delay_timer;
            break;
        }

        // Fx0A - LD Vx, K
        // All execution stops until a key is pressed, then the value of that key is stored in Vx.
        case 0x0A: {
            registers.reg_v[x] = wait_for_key_press();
            break;
        }

        // Fx15 - LD DT, Vx
        // The value of Vx is stored into delay timer.
        case 0x15: {
            registers.delay_timer = registers.reg_v[x];
            break;
        }

        // Fx18 - LD ST, Vx
        // The value of Vx is stored into sound timer.
        case 0x18: {
            registers.sound_timer = registers.reg_v[x];
            break;
        }

        // Fx1E - ADD I, Vx
        // The values of I and Vx are added, and the results are stored in I.
        case 0x1E: {
            registers.reg_i += registers.reg_v[x];
            break;
        }

        // Fx29 - LD F, Vx
        // The value of I is set to the location of the sprite corresponding to the value of Vx.
        case 0x29: {
            registers.reg_i = registers.reg_v[x] * CHIP8_SPRITE_HEIGHT;
            break;
        }

        // Fx33 - LD B, Vx
        // The interpreter takes the decimal value of Vx,
        // and places the hundreds digit in memory at location in I,
        // the tens digit at location I+1, and the ones digit at location I+2.
        case 0x33: {
            unsigned char hundreds = registers.reg_v[x] / 100;
            unsigned char tens = (registers.reg_v[x] / 10) % 10;
            unsigned char ones = registers.reg_v[x] % 10;
            memory.set_byte(registers.reg_i, hundreds);
            memory.set_byte(registers.reg_i + 1, tens);
            memory.set_byte(registers.reg_i + 2, ones);
            break;
        }

        // Fx55 - LD [I], Vx
        // Copy the values of registers V0 through Vx into memory, starting at the address in I.
        case 0x55: {
            for (int i=0; i<=x; i++) {
                memory.set_byte(registers.reg_i + i, registers.reg_v[i]);
            }
            break;
        }

        // Fx65 - LD Vx, [I]
        // Reads values from memory starting at location I into registers V0 through Vx.
        case 0x65: {
            for (int i=0; i<=x; i++) {
                registers.reg_v[i] = memory.get_byte(registers.reg_i + i);
            }
            break;
        }
    };
}


void Chip8::exec_extended(unsigned short opcode) {
    unsigned short nnn = opcode & 0x0FFF;
    unsigned char x = (opcode >> 8) & 0x000F;
    unsigned char y = (opcode >> 4) & 0x000F;
    unsigned char kk = opcode & 0x00FF;
    unsigned char n = opcode & 0x000F;

    switch (opcode & 0xF000) {
        // 1nnn - JP addr
        // Set the program counter to nnn
        case 0x1000: {
            registers.program_counter = nnn;
            break;
        }

        // 2nnn - CALL addr
        // Increments the stack pointer, then puts the current PC on the top of the stack.
        // The PC is then set to nnn.
        case 0x2000: {
            stack.push(registers.program_counter);
            registers.program_counter = nnn;
            break;
        }

        // 3xkk - SE Vx, byte
        // Compares register Vx to kk, and if they are equal, increments the program counter by 2.
        case 0x3000: {
            registers.program_counter += 2 * (registers.reg_v[x] == kk);
            break;
        }

        // 4xkk - SNE Vx, byte
        // Compares register Vx to kk, and if they are not equal, increments the program counter by 2.
        case 0x4000: {
            registers.program_counter += 2 * (registers.reg_v[x] != kk);
            break;
        }

        // 5xy0 - SE Vx, Vy
        // Compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
        case 0x5000: {
            registers.program_counter += 2 * (registers.reg_v[x] == registers.reg_v[y]);
            break;
        }

        // 6xkk - LD Vx, byte
        // Store the value kk into register Vx.
        case 0x6000: {
            registers.reg_v[x] = kk;
            break;
        }

        // 7xkk - ADD Vx, byte
        // Adds the value kk to the value of register Vx, then stores the result in Vx.
        case 0x7000: {
            registers.reg_v[x] += kk;
            break;
        }

        // 8 Series
        case 0x8000: {
            exec_extended_8_series(opcode);
            break;
        }

        // 9xy0 - SNE Vx, Vy
        // Compares register Vx to register Vy, and if they are not equal, increments the program counter by 2.
        case 0x9000: {
            registers.program_counter += 2 * (registers.reg_v[x] != registers.reg_v[y]);
            break;
        }

        // Annn - LD I, addr
        // The value of register I is set to nnn.
        case 0xA000: {
            registers.reg_i = nnn;
            break;
        }

        // Bnnn - JP V0, addr
        // The program counter is set to nnn plus the value of V0.
        case 0xB000: {
            registers.program_counter = nnn + registers.reg_v[0x00];
            break;
        }

        // Cxkk - RND Vx, byte
        // Generates a random number from 0 to 255, which is then ANDed with the value kk.
        // The results are stored in Vx.
        case 0xC000: {
            srand(clock());
            registers.reg_v[x] = (rand() % 255) & kk;
            break;
        }

        // Dxyn - DRW Vx, Vy, nibble
        // Reads n bytes from memory, starting at the address stored in I.
        // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
        // Sprites are XORed onto the existing screen.
        // If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
        // If part of the sprite is outside the coordinates of the display, it wraps around to the opposite side of the screen.
        case 0xD000: {
            const char *sprite = (const char *) &memory.memory[registers.reg_i];
            registers.reg_v[0x0F] = screen.draw_sprite(registers.reg_v[x], registers.reg_v[y], sprite, n);
            break;
        }

        // E Series
        case 0xE000: {
            exec_extended_E_series(opcode);
            break;
        }

        // F Series
        case 0xF000: {
            exec_extended_F_series(opcode);
            break;
        }
    };
}


void Chip8::exec(unsigned short opcode) {
    switch (opcode) {
        // 00E0 - CLS
        // Clear the display.
        case 0x00E0: {
            screen.clear();
            break;
        }

        // 00EE - RET
        // The interpreter sets the program counter to the address at the top of the stack.
        // Then subtracts 1 from the stack pointer.
        case 0x00EE: {
            registers.program_counter = stack.pop();
            break;
        }

        default:
            exec_extended(opcode);
    };
}

