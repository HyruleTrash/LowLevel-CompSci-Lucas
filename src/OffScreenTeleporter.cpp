//
// Created by Lucas on 15/09/2025.
//

#include "OffScreenTeleporter.h"

void OffScreenTeleporter::Update(sf::RenderWindow &window, Profiler &profiler, sf::Time &deltaTime) {
    Component::Update(window, profiler, deltaTime);
    auto screenSize = window.getSize();

    if (parent->position.x > screenSize.x)
        parent->position.x = 0;
    if (parent->position.x < 0)
        parent->position.x = screenSize.x;

    if (parent->position.y > screenSize.y)
        parent->position.y = 0;
    if (parent->position.y < 0)
        parent->position.y = screenSize.y;
}