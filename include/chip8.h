#ifndef CHIP8_H_
#define CHIP8_H_

#include <stddef.h>

#include "ram.h"
#include "registers.h"
#include "stack.h"
#include "keyboard.h"
#include "screen.h"


class Chip8 {
    private:
        void exec_extended(unsigned short opcode);
        void exec_extended_8_series(unsigned short opcode);
        void exec_extended_E_series(unsigned short opcode);
        void exec_extended_F_series(unsigned short opcode);
        char wait_for_key_press(void);
    public:
        Memory memory;
        Registers registers;
        Stack stack;
        Keyboard keyboard;
        Screen screen;

        Chip8(void);
        void load_code_from_buffer(const unsigned char *buffer, size_t size);
        void exec(unsigned short opcode);
};

#endif
