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
    EntityManager();
    ~EntityManager();
    void Update(sf::RenderWindow&);
    void AddEntity(Entity*);
    std::vector<Entity*> entities;
};


#endif //LOW_LEVEL_SETUP_ENTITYMANAGER_H