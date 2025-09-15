//
// Created by Lucas on 11/09/2025.
//

#include "Ball.h"
#include "Config.h"
#include <iostream>
#include <ostream>

Ball::Ball() : shape(radius) {
    position = sf::Vector2f(500, 500); // temp
    shape.setFillColor(color);
    shape.setPosition(position);
    body = RigidBody();
    AddComponent(&body);
}

void Ball::Render(sf::RenderWindow &window, Profiler &profiler, sf::Time& deltaTime) {
    PROFILE(profiler, "Ball Render");
    Entity::Render(window, profiler, deltaTime);
    window.draw(shape);
}


void Ball::Update(sf::RenderWindow& window, Profiler& profiler, sf::Time& deltaTime) {
    PROFILE(profiler, "Ball Update");
    Entity::Update(window, profiler, deltaTime);
    shape.setPosition(position);

    const auto OnKeyPressed = [&window, this](const sf::Event::KeyPressed& keyPressed)
    {
        if (keyPressed.code == sf::Keyboard::Key::W || keyPressed.code == sf::Keyboard::Key::Up)
            body.ApplyForce(Vector2::UP);
        if (keyPressed.code == sf::Keyboard::Key::A || keyPressed.code == sf::Keyboard::Key::Left)
            body.ApplyForce(Vector2::LEFT);
        if (keyPressed.code == sf::Keyboard::Key::S || keyPressed.code == sf::Keyboard::Key::Down)
            body.ApplyForce(Vector2::DOWN);
        if (keyPressed.code == sf::Keyboard::Key::D || keyPressed.code == sf::Keyboard::Key::Right)
            body.ApplyForce(Vector2::RIGHT);
    };
    window.handleEvents(OnKeyPressed);
}
