#include "pattern.h"
#include <stdlib.h>
#include <assert.h>


static bool tile_equals(const tile_s* a, const tile_s* b) {
    assert(a);
    assert(b);

    return a->color.r == b->color.r &&
           a->color.g == b->color.g &&
           a->color.b == b->color.b &&
           a->color.a == b->color.a;
}

void pattern_init(pattern_s* pattern, const pattern_desc_s* desc) {
    assert(pattern);
    assert(desc);
    assert(desc->n > 0);

    *pattern = (pattern_s){ 
        .n = desc->n,
        .num_tiles = desc->n * desc->n
    };

    pattern->tiles = malloc(sizeof(tile_s) * pattern->num_tiles);
    assert(pattern->tiles);

    for (size_t i = 0; i < pattern->num_tiles; i++) {
        pattern->tiles[i] = (tile_s){ 0 };
    }
}

void pattern_deinit(pattern_s* pattern) {
    assert(pattern);

    if (pattern->tiles)
        free(pattern->tiles);

    *pattern = (pattern_s){ 0 };
}

bool pattern_equals(const pattern_s* a, const pattern_s* b) {
    assert(a);
    assert(b);
    
    if (a == b) 
        return true;
    if (a->n != b->n) 
        return false;
    if (a->num_tiles != b->num_tiles)
        return false;

    for (size_t i = 0; i < a->num_tiles; i++) {
        if (!tile_equals(&a->tiles[i], &b->tiles[i]))
            return false;
    }

    return true;
}

void pattern_copy_from_surface(pattern_s* pattern, SDL_Surface* surface, unsigned int x, unsigned int y) {
    assert(pattern);
    assert(surface);
    assert(x + pattern->n <= (unsigned int)surface->w);
    assert(y + pattern->n <= (unsigned int)surface->h);
    
    int n = pattern->n;
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            color_s color;
            SDL_ReadSurfacePixel(surface, x + i, y + i, &color.r, &color.g, &color.b, &color.a);
            pattern->tiles[j * n + i].color = color;       
        }
    }
}

static int get_tile_index(int n, int ni, direction_e dir) {
    switch (dir) {
        case DIR_UP:
            return ni;
        case DIR_DOWN:
            return (n - 1) * n + ni;
        case DIR_LEFT:
            return ni * n; 
        case DIR_RIGHT:
            return ni * n + (n - 1);
        default:
            return -1;
    }
}

bool pattern_adjacent(const pattern_s* a, const pattern_s* b, direction_e dir) {
    assert(a);
    assert(b);
    assert(a->n == b->n);

    int n = a->n;
    for (int i = 0; i < n; i++) {
        int ai = get_tile_index(n, i, dir);
        int bi = get_tile_index(n, i, 
            dir == DIR_UP ? DIR_DOWN :
            dir == DIR_DOWN ? DIR_UP :
            dir == DIR_LEFT ? DIR_RIGHT :
            dir == DIR_RIGHT ? DIR_LEFT : -1);
        assert(ai >= 0 && ai < (n * n));
        assert(bi >= 0 && bi < (n * n));

        if (!tile_equals(&a->tiles[ai], &b->tiles[bi]))
            return false;
    }

    return true;
}
