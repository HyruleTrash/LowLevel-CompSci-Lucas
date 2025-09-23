//
// Created by Lucas on 18/09/2025.
//

#ifndef LOW_LEVEL_SETUP_TEXTURE_H
#define LOW_LEVEL_SETUP_TEXTURE_H
#include <list>
#include <string>
#include <vector>

#include "Pixel.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"


class Texture {
public:
    Texture(const sf::RectangleShape&, sf::Vector2u, sf::Vector2u);
    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);
    void Render(sf::RenderWindow& window, sf::Vector2f&) const;

public:
    sf::Vector2i size;
    std::vector<Pixel*> pixels{};
    std::string name;
};


#endif //LOW_LEVEL_SETUP_TEXTURE_H