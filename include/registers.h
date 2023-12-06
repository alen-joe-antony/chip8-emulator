#ifndef CHIP8_REGISTERS_H_
#define CHIP8_REGISTERS_H_

#include "config.h"


class Registers {
    public:
        unsigned char reg_v[CHIP8_TOTAL_DATA_REGISERS];
        unsigned short reg_i;
        unsigned char delay_timer;
        unsigned char sound_timer;
        unsigned short program_counter;

        Registers(void);
};

#endif
