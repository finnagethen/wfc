#include "tile.h"
#include <assert.h>

void tile_init(tile_s* tile, const tile_desc_s* desc) {
    assert(tile);
    assert(desc);
    assert(desc->surface && desc->rect);

    *tile = (tile_s){
        .id = desc->id,
        .surface = SDL_CreateSurface(desc->rect->w, desc->rect->h, SDL_PIXELFORMAT_RGBA32)
    };
    assert(tile->surface);

    SDL_BlitSurface(desc->surface, desc->rect, tile->surface, NULL);
}

void tile_deinit(tile_s* tile) {
    assert(tile);

    if (tile->surface)
        SDL_DestroySurface(tile->surface);

    for (size_t i = 0; i < _DIR_COUNT; i++)
        list_free(tile->adjacents[i]);

    *tile = (tile_s){ 0 };
}

void tile_add_adjacency(tile_s* tile, direction_e dir, const tile_s* adjacent) {
    assert(tile);
    assert(adjacent);

    tile->adjacents[dir] = list_push_front(tile->adjacents[dir], (size_t*)&adjacent->id);
}
