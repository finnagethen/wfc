#include "wfc.h"
#include "pattern.h"
#include <stdlib.h>
#include <assert.h>


void wfc_init(wfc_s* wfc, const wfc_desc_s* desc) {
    assert(wfc);
    assert(desc);
    assert(desc->n > 0);
    assert(desc->input_image_path);
    assert(desc->output_width > 0 && desc->output_height > 0);

    *wfc = (wfc_s){
        .n = desc->n,
        .num_patterns = (desc->output_width - desc->n + 1) * (desc->output_height - desc->n + 1)
    };

    // load input image
    wfc->input_surface = SDL_LoadBMP(desc->input_image_path);
    assert(wfc->input_surface);

    // create output surface
    wfc->output_surface = SDL_CreateSurface(desc->output_width, desc->output_height, SDL_PIXELFORMAT_RGBA32);
    assert(wfc->output_surface);

    // init patterns
    wfc->patterns = malloc(sizeof(pattern_s) * wfc->num_patterns);
    assert(wfc->patterns);

    for (size_t i = 0; i < wfc->num_patterns; i++) {
        pattern_init(&wfc->patterns[i], &(pattern_desc_s){ 
            .n = wfc->n 
        });
    }
}

void wfc_deinit(wfc_s* wfc) {
    assert(wfc);
    
    if (wfc->input_surface) 
        SDL_DestroySurface(wfc->input_surface);

    if (wfc->output_surface)
        SDL_DestroySurface(wfc->output_surface);

    if (wfc->patterns) {
        for (size_t i = 0; i < wfc->num_patterns; i++) {
            pattern_deinit(&wfc->patterns[i]);
        }
        free(wfc->patterns);
    }

    *wfc = (wfc_s){ 0 };
}

void wfc_extract_patterns(wfc_s* wfc) {
    assert(wfc);

    int width = wfc->input_surface->w;
    int height = wfc->input_surface->h;

    size_t index = 0;
    for (int y = 0; y <= height - wfc->n; y++) {
        for (int x = 0; x <= width - wfc->n; x++) {
            assert(index < wfc->num_patterns);
            pattern_copy_from_surface(&wfc->patterns[index], wfc->input_surface, x, y);
            index++;
        }
    }
}
