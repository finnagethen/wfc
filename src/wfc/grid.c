#include "grid.h"
#include <assert.h>

void grid_init(grid_s* grid, const grid_desc_s* desc) {
    assert(grid);
    assert(desc);
    assert(desc->width > 0 && desc->height > 0 && desc->cell_size > 0);

    *grid = (grid_s){
        .surface = SDL_CreateSurface(desc->width, desc->height, SDL_PIXELFORMAT_RGBA32),
        .rows = desc->height / desc->cell_size,
        .cols = desc->width / desc->cell_size,
    };
    assert(grid->surface);
}

void grid_deinit(grid_s* grid) {
    assert(grid);

    if (grid->surface)
        SDL_DestroySurface(grid->surface);

    *grid = (grid_s){ 0 };
}

void grid_set_subsurface(grid_s* grid, SDL_Surface* subsurface, int row, int col) {
    assert(grid);
    assert(subsurface);
    assert(row >= 0 && row < grid->rows);
    assert(col >= 0 && col < grid->cols);

    SDL_BlitSurface(subsurface, NULL, grid->surface, &(SDL_Rect){
        .x = col * subsurface->w,
        .y = row * subsurface->h,
        .w = subsurface->w,
        .h = subsurface->h,

    });
}
