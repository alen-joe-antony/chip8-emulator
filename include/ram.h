#ifndef CHIP8_MEMORY_H_
#define CHIP8_MEMORY_H_

#include "config.h"


class Memory {
    private:
        void assert_memory_in_bounds(int index);
    
    public:
        unsigned char memory[CHIP8_MEMORY_SIZE];

        Memory(void);
        void set_byte(int index, unsigned char value);
        unsigned char get_byte(int index);
        unsigned short get_word(int index);
};

#endif
