//
// Created by Lucas on 23/09/2025.
//

#ifndef LOW_LEVEL_SETUP_PIXEL_H
#define LOW_LEVEL_SETUP_PIXEL_H
#include <memory>

#include "SFML/Graphics/RectangleShape.hpp"

struct Pixel {
    bool state;
    std::shared_ptr<Pixel> neighbour100; // topLeft
    std::shared_ptr<Pixel> neighbour010; // topMiddle
    std::shared_ptr<Pixel> neighbour001; // topRight
    std::shared_ptr<Pixel> neighbour000; // Left
    std::shared_ptr<Pixel> neighbour111; // Right
    std::shared_ptr<Pixel> neighbour011; // BottomLeft
    std::shared_ptr<Pixel> neighbour101; // BottomMiddle
    std::shared_ptr<Pixel> neighbour110; // BottomRight
    std::unique_ptr<sf::RectangleShape> render;
};


#endif //LOW_LEVEL_SETUP_PIXEL_H