#include <assert.h>
#include <memory.h>

#include "stack.h"
#include "config.h"


Stack::Stack(void) {
    memset(stack, 0, sizeof(stack));
    stack_pointer = 0;    
}


void Stack::assert_stack_in_bounds(void) {
    assert(stack_pointer >= 0 && stack_pointer < CHIP8_STACK_DEPTH);
}


void Stack::push(unsigned short value) {
    assert_stack_in_bounds();
    stack[stack_pointer] = value;
    stack_pointer += 1;
}


unsigned short Stack::pop(void) {
    stack_pointer -= 1;
    assert_stack_in_bounds();
    return stack[stack_pointer];
}
