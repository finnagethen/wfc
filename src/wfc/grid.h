#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "utils/list.h"
#include "tile.h"

typedef struct cell {
    list_s* possible_tile_ids;
    bool is_collapsed;
} cell_s;

typedef struct grid {
    SDL_Surface* surface;
    int rows, cols;

    cell_s* cells;
    size_t num_cells;
} grid_s;

typedef struct grid_desc {
    int width, height;
    int cell_size;
} grid_desc_s;

void grid_init(grid_s* grid, const grid_desc_s* desc);
void grid_deinit(grid_s* grid);
void grid_set_subsurface(grid_s* grid, SDL_Surface* subsurface, int row, int col);
void grid_update(grid_s* grid, const tile_s* tiles, size_t num_tiles);
