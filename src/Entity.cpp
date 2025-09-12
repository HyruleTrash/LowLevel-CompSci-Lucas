//
// Created by Lucas on 12/09/2025.
//

#include "Entity.h"

#include "Component.h"

void Entity::Update(sf::RenderWindow &window, Profiler &profiler) {
    for (const auto component : components) {
        if (component->enabled)
            component->Update(window, profiler);
    }
}
