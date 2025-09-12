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

void EntityManager::Update(sf::RenderWindow& window, Profiler& profiler) {
    PROFILE(profiler, "EntityManager Update");
    for (auto ent : entities) {
        ent->Update(window, profiler);
    }
}

void EntityManager::AddEntity(Entity* entity) {
    entities.push_back(entity);
}
