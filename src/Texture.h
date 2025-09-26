//
// Created by Lucas on 18/09/2025.
//

#ifndef LOW_LEVEL_SETUP_TEXTURE_H
#define LOW_LEVEL_SETUP_TEXTURE_H
#include <list>
#include <string>
#include <vector>

#include "Pixel.h"
#include "Profiler.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"


class Texture {
public:
    Texture(const sf::RectangleShape&, const sf::Vector2u&, const sf::Vector2u&, Profiler&);
    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);
    void Update(sf::RenderWindow& window, sf::Vector2f&) const;
    void CalculateNextState() const;
    void Render(sf::RenderWindow& window, sf::Vector2f&) const;

public:
    sf::Vector2i size;
    Profiler* profiler;
    std::string name;
private:
    mutable std::vector<Pixel*> pixels;
};


#endif //LOW_LEVEL_SETUP_TEXTURE_H