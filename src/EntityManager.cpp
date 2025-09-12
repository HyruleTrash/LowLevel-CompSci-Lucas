//
// Created by Lucas on 11/09/2025.
//

#include "EntityManager.h"

EntityManager::EntityManager(sf::RenderWindow& window, Profiler& profiler) {
    entities = {};
    this->window = &window;
    this->profiler = &profiler;

}

EntityManager::~EntityManager() {
    for (auto ent : entities) {
        delete ent;
    }
}

void EntityManager::Update() {
    PROFILE(*profiler, "EntityManager Update");
    for (auto ent : entities) {
        ent->Render(*window, *profiler);
        ent->Update(*window, *profiler);
    }
}

void EntityManager::AddEntity(Entity* entity) {
    entities.push_back(entity);
    entity->Start(*window, *profiler);
}
