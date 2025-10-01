/*
 * Wave Function Collapse Algorithm
 *
 * Use a image as input to generate a new image with similar patterns.
 */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <assert.h>
#include <stdio.h> 
#include <unistd.h>

typedef struct state {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    struct {
        int width;
        int height;
    } size;

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

    state.texture = IMG_LoadTexture(state.renderer, image_path);
    assert(state.texture);

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

        // rendering
        SDL_SetRenderDrawColor(state.renderer, 255, 0, 255, 255);
        SDL_RenderClear(state.renderer);
        
        SDL_RenderTexture(state.renderer, state.texture, NULL, NULL);

        SDL_RenderPresent(state.renderer);
    }

    // deinit
    SDL_DestroyTexture(state.texture);
    SDL_DestroyWindow(state.window);
    SDL_DestroyRenderer(state.renderer);
    SDL_Quit();

    return 0;
}
