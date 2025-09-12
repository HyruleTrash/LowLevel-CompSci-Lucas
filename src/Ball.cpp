//
// Created by Lucas on 11/09/2025.
//

#include "Ball.h"

#include <iostream>
#include <ostream>

Ball::Ball() : shape(radius) {
    position = sf::Vector2f(200, 200); // temp
    shape.setFillColor(color);
    shape.setPosition(position);
}

Ball::~Ball() {
}

void Ball::Update(sf::RenderWindow& window, Profiler& profiler) {
    PROFILE(profiler, "Ball Update");
    Entity::Update(window, profiler);
    shape.setPosition(position);
    window.draw(shape);

    // std::cout << shape.getPointCount() << std::endl;
    // std::cout << shape.getRadius() << std::endl;
    // std::cout << shape.getFillColor().toInteger() << std::endl;
    // std::cout << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;
}
