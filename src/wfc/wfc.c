#include "wfc.h"
#include <SDL3_image/SDL_image.h>
#include <assert.h>
#include <stdlib.h>

void wfc_init(wfc_s* wfc, const wfc_desc_s* desc) {
    assert(wfc);
    assert(desc);
    assert(desc->path);
    assert(desc->tile_size > 0 && desc->output_width > 0 && desc->output_height > 0);

    *wfc = (wfc_s){
        .input_surface = IMG_Load(desc->path), 
        .tile_size = desc->tile_size,
    };
    assert(wfc->input_surface);

    // NOTE: is only true if the tiles are not wrapping around the edges!!
    size_t max_tiles = (wfc->input_surface->w - desc->tile_size) * (wfc->input_surface->h - desc->tile_size);
    wfc->num_tiles = max_tiles;
    wfc->tiles = malloc(sizeof(tile_s) * max_tiles);
    assert(wfc->tiles);

    grid_init(&wfc->grid, &(grid_desc_s){
        .width = desc->output_width,
        .height = desc->output_height,
        .cell_size = desc->tile_size,
    });
}

void wfc_deinit(wfc_s* wfc) {
    assert(wfc);

    if (wfc->input_surface)
        SDL_DestroySurface(wfc->input_surface);

    if (wfc->tiles) {
        for (size_t i = 0; i < wfc->num_tiles; i++) {
            tile_deinit(&wfc->tiles[i]);
        }
        free(wfc->tiles);
    }

    grid_deinit(&wfc->grid);

    *wfc = (wfc_s){ 0 };
}
