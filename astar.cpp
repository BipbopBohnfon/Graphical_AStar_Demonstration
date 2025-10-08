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
std::vector<grid_cell*> closed_set;

bool finished = false;

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

bool is_in_closed_set(grid_cell* cell) {
    for (int i = 0; i < closed_set.size(); i++) {
        if (closed_set[i] == cell) {
            return true;
        }
    }
    return false;
}

bool is_in_open_set(grid_cell* cell) {
    for (int i = 0; i < open_set.size(); i++) {
        if (open_set[i] == cell) {
            return true;
        }
    }
    return false;
}

int calc_distance(grid_cell from, grid_cell to) {
    return MOVE_WEIGHT * (abs(from.x_coord - to.x_coord) + abs(from.y_coord - to.y_coord));
}

void close_node(grid_cell *node) {
    if (node == nullptr) return;

    node->type = wall;

    for (int i = 0; i < open_set.size(); i++) {
        if (open_set[i] == node) {
            open_set.erase(open_set.begin() + i);
            break;
        }
    }

    for (int i = 0; i < closed_set.size(); i++) {
        if (closed_set[i] == node) {
            closed_set.erase(closed_set.begin() + i);
            break;
        }
    }
}

void activate_paths(grid_cell *traversed, grid_cell grid[HOR_CELLS][VERT_CELLS]) {
    if (traversed == target_node_loc) {
        return;
    }
    if (grid[traversed->x_coord - 1][traversed->y_coord].type == target) finished = true;
    if (grid[traversed->x_coord + 1][traversed->y_coord].type == target) finished = true;
    if (grid[traversed->x_coord][traversed->y_coord - 1].type == target) finished = true;
    if (grid[traversed->x_coord][traversed->y_coord + 1].type == target) finished = true;

    if (traversed->x_coord > 0) {
        if (grid[traversed->x_coord - 1][traversed->y_coord].type == path) {
            grid[traversed->x_coord - 1][traversed->y_coord].type = path_active;
            grid[traversed->x_coord - 1][traversed->y_coord].sf_RectP.setFillColor(sf::Color::Green);
            grid[traversed->x_coord - 1][traversed->y_coord].previous = traversed;
            grid[traversed->x_coord - 1][traversed->y_coord].g_cost = traversed->g_cost + MOVE_WEIGHT;
            grid[traversed->x_coord - 1][traversed->y_coord].h_cost = calc_distance(grid[traversed->x_coord - 1][traversed->y_coord], *target_node_loc);
            if (!is_in_open_set(&grid[traversed->x_coord - 1][traversed->y_coord])) {
                open_set.push_back(&grid[traversed->x_coord - 1][traversed->y_coord]);
            }
        }
    }
    if (traversed->x_coord < x_size - 1) {
        if (grid[traversed->x_coord + 1][traversed->y_coord].type == path) {
            grid[traversed->x_coord + 1][traversed->y_coord].type = path_active;
            grid[traversed->x_coord + 1][traversed->y_coord].sf_RectP.setFillColor(sf::Color::Green);
            grid[traversed->x_coord + 1][traversed->y_coord].previous = traversed;
            grid[traversed->x_coord + 1][traversed->y_coord].g_cost = traversed->g_cost + MOVE_WEIGHT;
            grid[traversed->x_coord + 1][traversed->y_coord].h_cost = calc_distance(grid[traversed->x_coord + 1][traversed->y_coord], *target_node_loc);
            if (!is_in_open_set(&grid[traversed->x_coord + 1][traversed->y_coord])) {
                open_set.push_back(&grid[traversed->x_coord + 1][traversed->y_coord]);
            }
        }
    }
    if (traversed->y_coord > 0) {
        if (grid[traversed->x_coord][traversed->y_coord - 1].type == path) {
            grid[traversed->x_coord][traversed->y_coord - 1].type = path_active;
            grid[traversed->x_coord][traversed->y_coord - 1].sf_RectP.setFillColor(sf::Color::Green);
            grid[traversed->x_coord][traversed->y_coord - 1].previous = traversed;
            grid[traversed->x_coord][traversed->y_coord - 1].g_cost = traversed->g_cost + MOVE_WEIGHT;
            grid[traversed->x_coord][traversed->y_coord - 1].h_cost = calc_distance(grid[traversed->x_coord][traversed->y_coord - 1], *target_node_loc);
            if (!is_in_open_set(&grid[traversed->x_coord][traversed->y_coord - 1])) {
                open_set.push_back(&grid[traversed->x_coord][traversed->y_coord - 1]);
            }
        }
    }
    if (traversed->y_coord < y_size - 1) {
        if (grid[traversed->x_coord][traversed->y_coord + 1].type == path) {
            grid[traversed->x_coord][traversed->y_coord + 1].type = path_active;
            grid[traversed->x_coord][traversed->y_coord + 1].sf_RectP.setFillColor(sf::Color::Green);
            grid[traversed->x_coord][traversed->y_coord + 1].previous = traversed;
            grid[traversed->x_coord][traversed->y_coord + 1].g_cost = traversed->g_cost + MOVE_WEIGHT;
            grid[traversed->x_coord][traversed->y_coord + 1].h_cost = calc_distance(grid[traversed->x_coord][traversed->y_coord + 1], *target_node_loc);
            if (!is_in_open_set(&grid[traversed->x_coord][traversed->y_coord + 1])) {
                open_set.push_back(&grid[traversed->x_coord][traversed->y_coord + 1]);
            }
        }
    }
}

bool run(grid_cell grid[HOR_CELLS][VERT_CELLS]) {
    if (open_set.empty()) {
        return false; // No path found
    }

    int cheapest_index = find_cheapest(grid);
    grid_cell* next = open_set[cheapest_index];

    // Check if we reached the target
    if (finished) {
        return true; // Path found!
    }

    open_set.erase(open_set.begin() + cheapest_index);
    closed_set.push_back(next);

    activate_paths(next, grid);

    return false; // Continue searching
}
