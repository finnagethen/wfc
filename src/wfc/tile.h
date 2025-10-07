#pragma once

#include <SDL3/SDL.h>
#include "utils/list.h"

typedef enum direction {
    DIR_UP,
    DIR_DOWN, 
    DIR_LEFT,
    DIR_RIGHT,
    _DIR_COUNT
} direction_e;

typedef struct tile {
    size_t id;
    SDL_Surface* surface;
    list_s* adjacents[_DIR_COUNT];
} tile_s;

typedef struct tile_desc {
    SDL_Surface* surface;
    const SDL_Rect* rect;
    size_t id;
} tile_desc_s;

void tile_init(tile_s* tile, const tile_desc_s* desc);
void tile_deinit(tile_s* tile);
void tile_add_adjacency(tile_s* tile, direction_e dir, const tile_s* adjacent);
