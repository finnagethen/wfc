#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "tile.h"
#include "grid.h"

typedef struct wfc {
    SDL_Surface* input_surface;

    grid_s grid;

    tile_s* tiles;
    size_t num_tiles;
    int tile_size;
} wfc_s;

typedef struct wfc_desc {
    const char* path;

    int tile_size;
    int output_width, output_height;
} wfc_desc_s;

void wfc_init(wfc_s* wfc, const wfc_desc_s* desc);
void wfc_deinit(wfc_s* wfc);

