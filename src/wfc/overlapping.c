#include "overlapping.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

void overlapping_extract_tiles(SDL_Surface* input_surface, tile_s* tiles, size_t num_tiles, int tile_size) {
    assert(input_surface);
    assert(tiles);
    assert(num_tiles > 0 && tile_size > 0);

    int width = input_surface->w;
    int height = input_surface->h;

    size_t index = 0;
    for (int y = 0; y <= height - tile_size; y++) {
        for (int x = 0; x <= width - tile_size; x++) {
            if (index >= num_tiles) {
                return;
            }
            tile_init(&tiles[index], &(tile_desc_s){ 
                .surface = input_surface, 
                .rect = &(SDL_Rect){ 
                    .x = x, 
                    .y = y, 
                    .w = tile_size, 
                    .h = tile_size 
                },
                .id = index
            });
            index++;
        }
    }
}

static bool pixel_eq(SDL_Surface* a, int x1, int y1, SDL_Surface* b, int x2, int y2) {
    uint8_t r1, g1, b1, a1;
    uint8_t r2, g2, b2, a2;

    SDL_ReadSurfacePixel(a, x1, y1, &r1, &g1, &b1, &a1);
    SDL_ReadSurfacePixel(b, x2, y2, &r2, &g2, &b2, &a2);

    return r1 == r2 && g1 == g2 && b1 == b2 && a1 == a2;
}

static bool is_machting_tile_vertical(const tile_s* a, const tile_s* b, int border_size) {
    int width = a->surface->w,
        height = a->surface->h;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < border_size; y++) {
            int x1 = x, 
                y1 = height - border_size + y, 
                x2 = x,
                y2 = y;
           if (!pixel_eq(a->surface, x1, y1, b->surface, x2, y2))
                return false; 
        }
    }
    return true;
}

static bool is_machting_tile_horizontal(const tile_s* a, const tile_s* b, int border_size) {
    int width = a->surface->w,
        height = a->surface->h;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < border_size; x++) {
            int x1 = width - border_size + x, 
                y1 = y, 
                x2 = x,
                y2 = y;
            if (!pixel_eq(a->surface, x1, y1, b->surface, x2, y2))
                return false;
        }
    }
    return true;
}

static bool is_matching_tile(const tile_s* a, const tile_s* b, direction_e dir, int border_size) {
    assert(a && b);
    assert(dir >= 0 && dir < _DIR_COUNT);
    assert(a->surface->w == b->surface->w && a->surface->h == b->surface->h);
    assert(border_size > 0 && border_size <= a->surface->w && border_size <= a->surface->h);

    switch (dir) {
        case DIR_UP:
            return is_machting_tile_vertical(a, b, border_size);    
        case DIR_DOWN:
            return is_machting_tile_vertical(b, a, border_size);
        case DIR_LEFT:
            return is_machting_tile_horizontal(a, b, border_size);
        case DIR_RIGHT:
            return is_machting_tile_horizontal(b, a, border_size);
        default:
            return false;
    }
}

void overlapping_calc_adjacency_rules(tile_s* tiles, size_t num_tiles, int border_size) {
    assert(tiles);
    assert(num_tiles > 0 && border_size > 0);

    for (size_t i = 0; i < num_tiles; i++) {
        for (size_t j = 0; j < num_tiles; j++) {
            for (direction_e dir = 0; dir < _DIR_COUNT; dir++) {
                if (is_matching_tile(&tiles[i], &tiles[j], dir, border_size)) {
                    tile_add_adjacency(&tiles[i], dir, &tiles[j]);
                }
            }
        }
    }
}
