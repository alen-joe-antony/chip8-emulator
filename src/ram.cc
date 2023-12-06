#include <assert.h>
#include <memory.h>

#include "config.h"
#include "ram.h"


Memory::Memory(void) {
    memset(memory, 0, sizeof(memory));
}


void Memory::assert_memory_in_bounds(int index) {
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}      


void Memory::set_byte(int index, unsigned char value) {
    assert_memory_in_bounds(index);
    memory[index] = value;
}     


unsigned char Memory::get_byte(int index) {
    assert_memory_in_bounds(index);
    return memory[index];
}   


unsigned short Memory::get_word(int index) {
    assert_memory_in_bounds(index);
    unsigned char byte1 = get_byte(index);
    unsigned char byte2 = get_byte(index + 1);
    return (byte1 << 8) | byte2;
}
