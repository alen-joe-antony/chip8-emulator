#ifndef CHIP8_KEYBOARD_H_
#define CHIP8_KEYBOARD_H_

#include <stdbool.h>

#include "config.h"


class Keyboard {
    private:
        const char *keymap;

        void assert_key_in_bounds(char key);

    public:
        bool keyboard[CHIP8_TOTAL_KEYS];

        Keyboard(void);
        char map(char key);
        void set_keymap(const char *keymap);
        void key_down(char key);
        void key_up(char key);
        bool is_key_down(char key);
};

#endif
