#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
// #include <SFML/Audio.hpp>

#include "astar.h"

grid_cell *source_node = nullptr, *target_node = nullptr;
bool started = false;
int tick = 0;

int main() {
    sf::RenderWindow window(sf::VideoMode({800,600}), "A* App Window");
    window.setFramerateLimit(60);

    grid_cell grid[HOR_CELLS][VERT_CELLS];

    for (int i = 0; i < HOR_CELLS; i++) {
        for (int j = 0; j < VERT_CELLS; j++) {
            grid[i][j].sf_RectP = sf::RectangleShape(sf::Vector2f(20.f, 20.f));
            grid[i][j].sf_RectP.setPosition(sf::Vector2f(25.f + i * 25.f, 25.f + j * 25.f));
            grid[i][j].x_coord = i;
            grid[i][j].y_coord = j;
            grid[i][j].type = path;
        }
    }

    sf::Font indicator_font;
    if (!indicator_font.openFromFile("OpenSans-Regular.ttf")) exit (0);
    sf::Text indicator_text(indicator_font, "", 24);
    indicator_text.setFont(indicator_font);
    indicator_text.setPosition(sf::Vector2f(100.f, 650.f));
    indicator_text.setFillColor(sf::Color::White);
    indicator_text.setCharacterSize(24);

    sf::Text space_to_start(indicator_font, "", 24);
    space_to_start.setFont(indicator_font);
    space_to_start.setPosition(sf::Vector2f(260.f, 580.f));
    space_to_start.setString("Press Spacebar to Start");
    space_to_start.setCharacterSize(24);

    while(window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        window.clear(sf::Color::Black);

        for (int i = 0; i < HOR_CELLS; i++) {
            for (int j = 0; j < VERT_CELLS; j++) {
                window.draw(grid[i][j].sf_RectP);
            }
        }

        if (source_node != nullptr && target_node != nullptr && !started) {
            window.draw(space_to_start);
        }

        bool done = false;
        if (started) {
            if (run(grid)) started = false;;
            //grid[rand() % HOR_CELLS][rand() % VERT_CELLS].sf_RectP.setFillColor(sf::Color::Red);
        } else if (source_node != nullptr && target_node != nullptr && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !started && !done) {
            started = true;
            setup(HOR_CELLS, VERT_CELLS, target_node);
            add_source(source_node);
        }

        // Mouse position and corresponding cell
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        int x_cell = (mouse_pos.x - 20) / 25;
        int y_cell = (mouse_pos.y - 20) / 25;
        if (x_cell >= 0 && x_cell < HOR_CELLS && y_cell >= 0 && y_cell < VERT_CELLS) {
            char cell_string[50];
            sprintf(cell_string, "Cell: %d, %d\nG: %d, H: %d", x_cell, y_cell, grid[x_cell][y_cell].g_cost, grid[x_cell][y_cell].h_cost);
            indicator_text.setString(cell_string);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (source_node == nullptr) {
                    grid[x_cell][y_cell].type = source;
                    source_node = &grid[x_cell][y_cell];
                    source_node->type = source;
                    source_node->sf_RectP.setFillColor(sf::Color::Blue);
                } else if (target_node == nullptr && grid[x_cell][y_cell].type != source) {
                    grid[x_cell][y_cell].type = target;
                    target_node = &grid[x_cell][y_cell];
                    target_node->type = target;
                    target_node->sf_RectP.setFillColor(sf::Color::Cyan);
                } else if (grid[x_cell][y_cell].type != source && grid[x_cell][y_cell].type != target) {
                    grid[x_cell][y_cell].type = wall;
                    grid[x_cell][y_cell].sf_RectP.setFillColor(sf::Color::Black);
                }
            }
        }

        window.draw(indicator_text);

        window.display();
        tick++;
    }

    return 0;
}
