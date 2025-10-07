#pragma once

#include "tile.h"

void overlapping_extract_tiles(SDL_Surface* input_surface, tile_s* tiles, size_t num_tiles, int tile_size);
void overlapping_calc_adjacency_rules(tile_s* tiles, size_t num_tiles, int border_size);
