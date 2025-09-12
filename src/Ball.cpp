//
// Created by Lucas on 11/09/2025.
//

#include "Ball.h"

#include <iostream>
#include <ostream>

Ball::Ball() : shape(radius) {
    position = sf::Vector2f(20, 20); // temp
    shape.setFillColor(color);
    shape.setPosition(position);
}

Ball::~Ball() {
}

void Ball::Render(sf::RenderWindow &window, Profiler &profiler) {
    PROFILE(profiler, "Ball Render");
    Entity::Render(window, profiler);
    window.draw(shape);
}

void Ball::Update(sf::RenderWindow& window, Profiler& profiler) {
    PROFILE(profiler, "Ball Update");
    Entity::Update(window, profiler);
    shape.setPosition(position);
}
