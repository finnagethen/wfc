/*
 * Wave Function Collapse Algorithm
 *
 * Use an image as input to generate a new image with similar patterns.
 */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <assert.h>
#include <stdio.h> 
#include <unistd.h>

#include "wfc/wfc.h"
#include "wfc/overlapping.h"


typedef struct state {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    struct {
        int width;
        int height;
    } size;
    
    wfc_s wfc;
    
    bool quit;
} state_s;

void usage(const char* prog_name) {
    fprintf(stderr, "Usage: %s -i <input_image_path> -s <width>x<height>\n", prog_name);
}

int main(int argc, char* const* argv) {
    // parse command line arguments
    char* image_path = NULL;
    int output_width = 0;
    int output_height = 0;

    int option;
    while ((option = getopt(argc, argv, ":i:s:")) != -1) {
        switch (option) {
            case 'i':
                image_path = optarg;
                break;
            case 's':
                if (sscanf(optarg, "%dx%d", &output_width, &output_height) != 2) {
                    usage(argv[0]);
                    return 1;
                }
                break;
            case '?':
            case ':':
            default:
                usage(argv[0]);
                return 1;
        }
    }

    if (!image_path || output_width <= 0 || output_height <= 0) {
        usage(argv[0]);
        return 1;
    }

    // init state
    state_s state = (state_s) {
        .size = {
            .width = output_width,
            .height = output_height,
        },
    };

    // init SDL
    assert(SDL_Init(SDL_INIT_VIDEO));

    assert(SDL_CreateWindowAndRenderer(
        "Wave Function Collapse",
        state.size.width, state.size.height,
        0, 
        &state.window, &state.renderer)
    );

    state.texture = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, state.size.width, state.size.height);
    assert(state.texture);

    // wfc init
    wfc_init(&state.wfc, &(wfc_desc_s){
        .path = image_path, 
        .output_width = output_width,
        .output_height = output_height,
        .tile_size = 3,
    });
    overlapping_extract_tiles(state.wfc.input_surface, state.wfc.tiles, state.wfc.num_tiles, state.wfc.tile_size);
    overlapping_calc_adjacency_rules(state.wfc.tiles, state.wfc.num_tiles, 3);
    
    // show tiles for testing
    printf("Done extracting %zu tiles\n", state.wfc.num_tiles);
    tile_s tile = state.wfc.tiles[20];
    list_s* adj = tile.adjacents[DIR_RIGHT];
    int count = 0;
    while (adj) {
        size_t* id = (size_t*)adj->data;
        grid_set_subsurface(&state.wfc.grid, state.wfc.tiles[*id].surface, 0, count++);
        adj = adj->next;
    }
    
    // main loop
    SDL_Event event;
    while (!state.quit) {
        // event handling
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    state.quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                        state.quit = true;
                    }
                    break; 
                default:
                    break;
            }
        }
        
        // updating
        SDL_UpdateTexture(state.texture, NULL, state.wfc.grid.surface->pixels, state.wfc.grid.surface->pitch);

        // rendering
        SDL_SetRenderDrawColor(state.renderer, 255, 0, 255, 255);
        SDL_RenderClear(state.renderer);
        SDL_RenderTexture(state.renderer, state.texture, NULL, NULL);
        SDL_RenderPresent(state.renderer);
    }

    // deinit
    wfc_deinit(&state.wfc);
    SDL_DestroyTexture(state.texture);
    SDL_DestroyWindow(state.window);
    SDL_DestroyRenderer(state.renderer);
    SDL_Quit();

    return 0;
}
