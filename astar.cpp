//
// Created by bipbop on 10/17/23.
//

#include "astar.h"

// DELETE THIS
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <vector>

std::vector<grid_cell*> open_set;

int x_size;
int y_size;
grid_cell *target_node_loc;

void setup(int in_x_size, int in_y_size, grid_cell *in_target_node) {
    x_size = in_x_size;
    y_size = in_y_size;
    target_node_loc = in_target_node;
}

void add_source(grid_cell *source_node) {
    if (!open_set.empty() || source_node == nullptr) {
        perror("Something went terribly wrong. Check add_source.");
        exit(-1);
    }
    open_set.push_back(source_node);
}

// Find the cheapest tile in the open set
int find_cheapest(grid_cell grid[HOR_CELLS][VERT_CELLS]) {
    if (open_set.empty()) {
        perror("Something went terribly wrong. Open Set was Empty. Check find_cheapest.");
        exit(-1);
    }
    int lowest_f = INT_MAX;
    int index = 0;
    for (int i = 0; i < open_set.size(); i++) {
        int f_cost = open_set[i]->g_cost + open_set[i]->h_cost;
        if (f_cost < lowest_f) {
            lowest_f = f_cost;
            index = i;
        }
    }
    return index;
}

int calc_distance(grid_cell from, grid_cell to) {
    return MOVE_WEIGHT * (abs(from.x_coord - to.x_coord) + abs(from.y_coord - to.y_coord));
}

void activate_paths(grid_cell *traversed, grid_cell grid[HOR_CELLS][VERT_CELLS]) {
    if (traversed->x_coord > 0) {
        if (grid[traversed->x_coord - 1][traversed->y_coord].type == path) {
            grid[traversed->x_coord - 1][traversed->y_coord].type = path_active;
            grid[traversed->x_coord - 1][traversed->y_coord].sf_RectP.setFillColor(sf::Color::Green);
            grid[traversed->x_coord - 1][traversed->y_coord].previous = traversed;
            grid[traversed->x_coord - 1][traversed->y_coord].g_cost = traversed->g_cost + MOVE_WEIGHT;
            grid[traversed->x_coord - 1][traversed->y_coord].h_cost = calc_distance(grid[traversed->x_coord - 1][traversed->y_coord], *target_node_loc);
            open_set.push_back(&grid[traversed->x_coord - 1][traversed->y_coord]);
        }
    }
    if (traversed->x_coord < x_size - 1) {
        grid[traversed->x_coord + 1][traversed->y_coord].type = path_active;
        grid[traversed->x_coord + 1][traversed->y_coord].sf_RectP.setFillColor(sf::Color::Green);
        grid[traversed->x_coord + 1][traversed->y_coord].previous = traversed;
        grid[traversed->x_coord + 1][traversed->y_coord].g_cost = traversed->g_cost + MOVE_WEIGHT;
        grid[traversed->x_coord + 1][traversed->y_coord].h_cost = calc_distance(grid[traversed->x_coord + 1][traversed->y_coord], *target_node_loc);
        open_set.push_back(&grid[traversed->x_coord + 1][traversed->y_coord]);
    }
    if (traversed->y_coord > 0) {
        grid[traversed->x_coord][traversed->y_coord - 1].type = path_active;
        grid[traversed->x_coord][traversed->y_coord - 1].sf_RectP.setFillColor(sf::Color::Green);
        grid[traversed->x_coord][traversed->y_coord - 1].previous = traversed;
        grid[traversed->x_coord][traversed->y_coord - 1].g_cost = traversed->g_cost + MOVE_WEIGHT;
        grid[traversed->x_coord][traversed->y_coord - 1].h_cost = calc_distance(grid[traversed->x_coord][traversed->y_coord - 1], *target_node_loc);
        open_set.push_back(&grid[traversed->x_coord][traversed->y_coord - 1]);
    }
    if (traversed->y_coord < y_size - 1) {
        grid[traversed->x_coord][traversed->y_coord + 1].type = path_active;
        grid[traversed->x_coord][traversed->y_coord + 1].sf_RectP.setFillColor(sf::Color::Green);
        grid[traversed->x_coord][traversed->y_coord + 1].previous = traversed;
        grid[traversed->x_coord][traversed->y_coord + 1].g_cost = traversed->g_cost + MOVE_WEIGHT;
        grid[traversed->x_coord][traversed->y_coord + 1].h_cost = calc_distance(grid[traversed->x_coord][traversed->y_coord + 1], *target_node_loc);
        open_set.push_back(&grid[traversed->x_coord][traversed->y_coord + 1]);
    }
}

grid_cell* run(grid_cell grid[HOR_CELLS][VERT_CELLS]) {
    int cheapest_index = find_cheapest(grid);
    grid_cell* next = open_set[cheapest_index];
    open_set.erase(open_set.begin() + cheapest_index);
    activate_paths(next, grid);


    return &grid[rand() % x_size][rand() % y_size];
}
