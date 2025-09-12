//
// Created by Lucas on 11/09/2025.
//

#include "EntityManager.h"

EntityManager::EntityManager() {
    entities = {};
}

EntityManager::~EntityManager() {
    for (auto ent : entities) {
        delete ent;
    }
}

void EntityManager::Update(sf::RenderWindow& window) {
    for (auto ent : entities) {
        ent->Update(window);
    }
}

void EntityManager::AddEntity(Entity* entity) {
    entities.push_back(entity);
}
