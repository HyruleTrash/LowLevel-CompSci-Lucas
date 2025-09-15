//
// Created by Lucas on 12/09/2025.
//

#ifndef LOW_LEVEL_SETUP_COMPONENT_H
#define LOW_LEVEL_SETUP_COMPONENT_H
#include "Entity.h"
#include "Profiler.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class Component {
public:
    virtual void Update(sf::RenderWindow&, Profiler&, sf::Time& deltaTime) {};
public:
    Entity* parent;
    bool enabled = true;
};


#endif //LOW_LEVEL_SETUP_COMPONENT_H