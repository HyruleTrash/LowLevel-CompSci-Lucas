//
// Created by Lucas on 11/09/2025.
//

#ifndef LOW_LEVEL_SETUP_ENTITY_H
#define LOW_LEVEL_SETUP_ENTITY_H
#include "Profiler.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Entity {
public:
    Entity() = default;
    ~Entity() = default;
    virtual void Update(sf::RenderWindow&, Profiler&) {};
public:
    sf::Vector2<float> position;
};


#endif //LOW_LEVEL_SETUP_ENTITY_H