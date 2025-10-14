#include "wfc.h"
#include <SDL3_image/SDL_image.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

static void wfc_cells_init(wfc_s* wfc) {
    assert(wfc);

    for (size_t i = 0; i < wfc->grid.num_cells; i++) {
        cell_s* cell = &wfc->grid.cells[i];
        cell->possible_tile_ids = NULL;
        for (size_t t = 0; t < wfc->num_tiles; t++) {
            cell->possible_tile_ids = list_push_back(cell->possible_tile_ids, &wfc->tiles[t].id);
        }
        cell->entropy = wfc->num_tiles;
        cell->is_collapsed = false;
    }
}

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

    wfc_cells_init(wfc);

    srand((unsigned int)time(NULL));
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

static cell_s* wfc_get_next_cell(wfc_s* wfc) {
    assert(wfc);

    cell_s* best_cell = NULL;

    for (size_t i = 0; i < wfc->grid.num_cells; i++) {
        cell_s* cell = &wfc->grid.cells[i];
        if (cell->is_collapsed)
            continue;
        
        if (!best_cell || cell->entropy < best_cell->entropy)
            best_cell = cell;
    }

    return best_cell;
}

static void wfc_collapse_cell(wfc_s* wfc, cell_s* cell) {
    assert(wfc && cell);
    assert(cell->possible_tile_ids);
    assert(cell->entropy > 0);
    
    // choose a random tile from the possible tiles
    size_t choice = rand() % cell->entropy;
    list_s* curr = cell->possible_tile_ids;
    for (size_t i = 0; i < choice; i++) {
        curr = curr->next;
        assert(curr);
    }

    // collapse to that tile
    size_t* chosen_tile_id = (size_t*)curr->data;
    list_free(cell->possible_tile_ids);
    cell->possible_tile_ids = NULL;
    cell->possible_tile_ids = list_push_back(cell->possible_tile_ids, chosen_tile_id);
    cell->is_collapsed = true;
    cell->entropy = 0;
}

static void wfc_propagate_constraints(wfc_s* wfc, cell_s* cell) {
    assert(wfc && cell);
    // TODO
}

void wfc_update(wfc_s* wfc) {
    assert(wfc);
    
    cell_s* cell = wfc_get_next_cell(wfc);
    if (!cell)
        return; // all cells are collapsed

    wfc_collapse_cell(wfc, cell);

    grid_update(&wfc->grid, wfc->tiles, wfc->num_tiles);
}
