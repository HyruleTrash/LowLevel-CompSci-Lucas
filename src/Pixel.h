//
// Created by Lucas on 23/09/2025.
//

#ifndef LOW_LEVEL_SETUP_PIXEL_H
#define LOW_LEVEL_SETUP_PIXEL_H
#include <memory>

#include "SFML/Graphics/RectangleShape.hpp"

struct Pixel {
    bool state;
    Pixel* neighbour100; // topLeft
    Pixel* neighbour010; // topMiddle
    Pixel* neighbour001; // topRight
    Pixel* neighbour000; // Left
    Pixel* neighbour111; // Right
    Pixel* neighbour011; // BottomLeft
    Pixel* neighbour101; // BottomMiddle
    Pixel* neighbour110; // BottomRight
    std::unique_ptr<sf::RectangleShape> render;
};


#endif //LOW_LEVEL_SETUP_PIXEL_H