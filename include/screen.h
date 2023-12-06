#ifndef CHIP8_SCREEN_H_
#define CHIP8_SCREEN_H_

#include <stdbool.h>

#include "config.h"


class Screen {
    private:
        void assert_pixels_in_bounds(int x, int y);

    public:
        bool pixels[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];

        Screen(void);
        void clear(void);
        void set(int x, int y);
        bool is_set(int x, int y);
        bool draw_sprite(int x, int y, const char *sprite, int height);
};

#endif
