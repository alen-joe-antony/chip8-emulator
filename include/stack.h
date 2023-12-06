#ifndef CHIP8_STACK_H_
#define CHIP8_STACK_H_

#include "config.h"


class Stack {
    private:
        void assert_stack_in_bounds(void);

    public:
        unsigned short stack[CHIP8_STACK_DEPTH];
        unsigned char stack_pointer;

        Stack(void);
        void push(unsigned short value);
        unsigned short pop(void);
};

#endif
