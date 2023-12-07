#include <cassert>
#include <cstring>

#include "keyboard.h"
#include "config.h"


Keyboard::Keyboard(void) {
    memset(keyboard, 0, sizeof(keyboard));
    keymap = NULL;
}


void Keyboard::assert_key_in_bounds(unsigned char vkey) {
    assert(vkey >= 0 && vkey < CHIP8_TOTAL_KEYS);
}


void Keyboard::set_keymap(const char *keymap) {
    this->keymap = keymap;
}


char Keyboard::map(char key) {
    for (int i=0; i<CHIP8_TOTAL_KEYS; i++) {
        if (keymap[i] == key) {
            return i;
        }
    }
    return  -1;
}


void Keyboard::key_down(unsigned char vkey) {
    assert_key_in_bounds(vkey);
    keyboard[vkey] = true;
}


void Keyboard::key_up(unsigned char vkey) {
    assert_key_in_bounds(vkey);
    keyboard[vkey] = false;
}


bool Keyboard::is_key_down(unsigned char vkey) {
    assert_key_in_bounds(vkey);
    return keyboard[vkey];
}
