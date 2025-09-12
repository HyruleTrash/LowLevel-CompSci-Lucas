//
// Created by Lucas on 11/09/2025.
//

#ifndef LOW_LEVEL_SETUP_ENTITYMANAGER_H
#define LOW_LEVEL_SETUP_ENTITYMANAGER_H
#include "Entity.h"
#include <vector>

#include "SFML/Graphics/RenderWindow.hpp"

class EntityManager {
public:
    EntityManager(sf::RenderWindow&, Profiler&);
    ~EntityManager();
    void Update();
    void AddEntity(Entity*);
    std::vector<Entity*> entities;
private:
    sf::RenderWindow* window;
    Profiler* profiler;
};


#endif //LOW_LEVEL_SETUP_ENTITYMANAGER_H