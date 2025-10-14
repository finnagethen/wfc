#include "grid.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

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

    grid->num_cells = grid->rows * grid->cols;
    grid->cells = malloc(sizeof(cell_s) * grid->num_cells);
    assert(grid->cells);
    memset(grid->cells, 0, sizeof(cell_s) * grid->num_cells);
}

void grid_deinit(grid_s* grid) {
    assert(grid);

    if (grid->surface)
        SDL_DestroySurface(grid->surface);
    if (grid->cells) {
        for (size_t i = 0; i < grid->num_cells; i++) {
            cell_s* cell = &grid->cells[i];
            if (cell->possible_tile_ids)
                list_free(cell->possible_tile_ids);
        }
        free(grid->cells);
    }

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

void grid_update(grid_s* grid, const tile_s* tiles, size_t num_tiles) {
    assert(grid && tiles);

    for (int r = 0; r < grid->rows; r++) {
        for (int c = 0; c < grid->cols; c++) {
            cell_s* cell = &grid->cells[r * grid->cols + c];
            if (cell->is_collapsed) {
                assert(cell->possible_tile_ids);
                size_t tile_id = *(size_t*)cell->possible_tile_ids->data;
                assert(tile_id < num_tiles);
                grid_set_subsurface(grid, tiles[tile_id].surface, r, c);
            }
        }
    }
}
