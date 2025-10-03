//
// Created by Lucas on 18/09/2025.
//

#ifndef LOW_LEVEL_SETUP_TEXTURE_H
#define LOW_LEVEL_SETUP_TEXTURE_H
#include <bitset>
#include <list>
#include <string>
#include <vector>

#include "PixelNeighbours.h"
#include "Profiler.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include  "DefaultValues.h"


class Texture {
public:
    Texture(const sf::RectangleShape&, const sf::Vector2u&, Profiler&);
    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);
    void Update(sf::RenderWindow& window) const;
    void CalculateNextState() const;
    void Render(sf::RenderWindow& window) const;

public:
    Profiler* profiler;
private:
    std::vector<std::unique_ptr<sf::RectangleShape>> renders;
    std::vector<std::unique_ptr<PixelNeighbours>> neighboursLookup;
    mutable std::bitset<TEX_SIZE> states;
};


#endif //LOW_LEVEL_SETUP_TEXTURE_H