#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <SDL3/SDL.h>


typedef struct color {
    uint8_t r, g, b, a;
} color_s;

typedef struct tile {
    color_s color;
} tile_s;

typedef struct pattern {
    int n;
    size_t num_tiles;
    tile_s* tiles;
} pattern_s;

typedef struct pattern_desc {
    int n; // nxn pattern
} pattern_desc_s;

typedef enum direction {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} direction_e;

void pattern_init(pattern_s* pattern, const pattern_desc_s* desc);
void pattern_deinit(pattern_s* pattern);
bool pattern_equals(const pattern_s* a, const pattern_s* b); // TODO: check equality with hash
void pattern_copy_from_surface(pattern_s* pattern, SDL_Surface* surface, unsigned int x, unsigned int y);
bool pattern_adjacent(const pattern_s* a, const pattern_s* b, direction_e dir);

