#include <memory.h>

#include "registers.h"

Registers::Registers(void) {
    memset(reg_v, 0, sizeof(reg_v));
    reg_i = 0x0000;
    delay_timer = 0x00;
    sound_timer = 0x00;
    program_counter = 0x0000;
}
