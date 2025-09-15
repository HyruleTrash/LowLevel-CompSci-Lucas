//
// Created by Lucas on 15/09/2025.
//

#ifndef LOW_LEVEL_SETUP_CONFIG_H
#define LOW_LEVEL_SETUP_CONFIG_H
#include "SFML/System/Vector2.hpp"

namespace Vector2 {
    constexpr auto ZERO  = sf::Vector2f(0,  0);
    constexpr auto UP = sf::Vector2f(0, -1);
    constexpr auto DOWN = sf::Vector2f(0, 1);
    constexpr auto LEFT = sf::Vector2f(-1, 0);
    constexpr auto RIGHT = sf::Vector2f(1, 0);
}

class Config {
};

#endif //LOW_LEVEL_SETUP_CONFIG_H