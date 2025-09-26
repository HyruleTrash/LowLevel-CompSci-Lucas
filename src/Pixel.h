//
// Created by Lucas on 23/09/2025.
//

#ifndef LOW_LEVEL_SETUP_PIXEL_H
#define LOW_LEVEL_SETUP_PIXEL_H
#include <memory>

#include "SFML/Graphics/RectangleShape.hpp"

struct Pixel {
    bool alive;
    std::unique_ptr<sf::RectangleShape> render;
    int aliveNeighbourCount{0};
    std::vector<Pixel*> neighbours;
};


#endif //LOW_LEVEL_SETUP_PIXEL_H