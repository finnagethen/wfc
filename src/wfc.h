#pragma once

#include <SDL3/SDL.h>
#include "pattern.h"

typedef struct wfc {
    int n;
    size_t num_patterns;
    pattern_s* patterns;

    SDL_Surface* input_surface;
    SDL_Surface* output_surface;
} wfc_s;

typedef struct wfc_desc {
    int n;  // nxn pattern
    const char* input_image_path;
    int output_width, output_height;
} wfc_desc_s;

void wfc_init(wfc_s* wfc, const wfc_desc_s* desc);
void wfc_deinit(wfc_s* wfc);
void wfc_extract_patterns(wfc_s* wfc);
