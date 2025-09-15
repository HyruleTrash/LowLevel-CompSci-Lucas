//
// Created by Lucas on 12/09/2025.
//

#include "Entity.h"

#include "Component.h"

void Entity::Update(sf::RenderWindow &window, Profiler &profiler, sf::Time& deltaTime) {
    for (const auto component : components) {
        if (component->enabled)
            component->Update(window, profiler, deltaTime);
    }
}

void Entity::AddComponent(Component* component) {
    component->parent = this;
    components.push_back(component);
}
