#include <cassert>
#include <cstring>
#include <cstdbool>

#include "screen.h"
#include "config.h"


Screen::Screen(void) {
    memset(pixels, 0, sizeof(pixels));
}


void Screen::assert_pixels_in_bounds(int x, int y) {
    assert(x >= 0 && y < CHIP8_DISPLAY_WIDTH);
    assert(y >= 0 && y < CHIP8_DISPLAY_HEIGHT);
}


void Screen::clear(void) {
    memset(pixels, 0, sizeof(pixels));
}


void Screen::set(int x, int y) {
    assert_pixels_in_bounds(x, y);
    pixels[y][x] = true;
}


bool Screen::is_set(int x, int y) {
    assert_pixels_in_bounds(x, y);
    return pixels[y][x];
}


bool Screen::draw_sprite(int x, int y, const char *sprite, int height) {
    bool pixel_collision = false;
    
    for (int ly=0; ly<height; ly++) {
        char sprite_byte = sprite[ly];
        for (int lx=0; lx<8; lx++) {
            if ((sprite_byte & (0b10000000 >> lx)) == 0) {
                continue;
            }
            // Using modulo to warp around when the the sprite overflows
            pixel_collision = pixels[(ly+y) % CHIP8_DISPLAY_HEIGHT][(lx+x) % CHIP8_DISPLAY_WIDTH];
            pixels[(ly+y) % CHIP8_DISPLAY_HEIGHT][(lx+x) % CHIP8_DISPLAY_WIDTH] ^= true;
        }
    }
    return pixel_collision;;
}
