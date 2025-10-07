#pragma once

#include <SDL3/SDL.h>

typedef struct grid {
    SDL_Surface* surface;
    int rows, cols;
} grid_s;

typedef struct grid_desc {
    int width, height;
    int cell_size;
} grid_desc_s;

void grid_init(grid_s* grid, const grid_desc_s* desc);
void grid_deinit(grid_s* grid);
void grid_set_subsurface(grid_s* grid, SDL_Surface* subsurface, int row, int col);
