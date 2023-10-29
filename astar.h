//
// Created by bipbop on 10/17/23.
//

#ifndef ASTARPROJECT_ASTAR_H
#define ASTARPROJECT_ASTAR_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <cstdint>

#define HOR_CELLS 30
#define VERT_CELLS 22

#define MOVE_WEIGHT 10

enum cell_type {
    path,
    path_active,
    path_traversed,
    wall,
    source,
    target
};

typedef struct grid_cell {
    sf::RectangleShape sf_RectP; // Type sf::rectangleShape
    void* previous = nullptr; // Type grid_cell
    cell_type type = path;
    uint32_t g_cost = 0;
    uint32_t h_cost = 0;
    // F cost is equal to g + h
    uint16_t x_coord = 0;
    uint16_t y_coord = 0;
} grid_cell;

void setup(int in_x_size, int in_y_size, grid_cell *in_target_node);
void add_source(grid_cell *source_node);
grid_cell* run(grid_cell in_grid[HOR_CELLS][VERT_CELLS]);

#endif //ASTARPROJECT_ASTAR_H
