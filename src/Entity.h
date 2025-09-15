//
// Created by Lucas on 11/09/2025.
//

#ifndef LOW_LEVEL_SETUP_ENTITY_H
#define LOW_LEVEL_SETUP_ENTITY_H
#include "Profiler.hpp"
#include <vector>

class Component;

class Entity {
public:
    Entity() = default;
    ~Entity() = default;
    virtual void Start(sf::RenderWindow&, Profiler&) {};
    virtual void Render(sf::RenderWindow&, Profiler&, sf::Time& deltaTime) {};
    virtual void Update(sf::RenderWindow&, Profiler&, sf::Time& deltaTime);
    void AddComponent(Component*);
public:
    sf::Vector2<float> position;
    std::vector<Component*> components;
};


#endif //LOW_LEVEL_SETUP_ENTITY_H