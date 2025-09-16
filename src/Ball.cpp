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
    teleporter = OffScreenTeleporter();
    AddComponent(&body);
    AddComponent(&teleporter);
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

    if (body.velocity.lengthSquared() > 0) {
        body.ApplyForce(-body.velocity.normalized() * Physics::AIR_DRAG);
    }
    std::cout << body.velocity.x << ", " << body.velocity.y << std::endl;

    const auto OnKeyPressed = [&window, this](const sf::Event::KeyPressed& keyPressed)
    {
        const float impulseSpeed{800};
        const float forceSpeed{400};
        if (keyPressed.shift) {
            if (keyPressed.code == sf::Keyboard::Key::W || keyPressed.code == sf::Keyboard::Key::Up)
                body.ApplyImpulse(Vector2::UP * impulseSpeed);
            if (keyPressed.code == sf::Keyboard::Key::A || keyPressed.code == sf::Keyboard::Key::Left)
                body.ApplyImpulse(Vector2::LEFT * impulseSpeed);
            if (keyPressed.code == sf::Keyboard::Key::S || keyPressed.code == sf::Keyboard::Key::Down)
                body.ApplyImpulse(Vector2::DOWN * impulseSpeed);
            if (keyPressed.code == sf::Keyboard::Key::D || keyPressed.code == sf::Keyboard::Key::Right)
                body.ApplyImpulse(Vector2::RIGHT * impulseSpeed);
        }
        if (keyPressed.code == sf::Keyboard::Key::W || keyPressed.code == sf::Keyboard::Key::Up)
            body.ApplyForce(Vector2::UP * forceSpeed);
        if (keyPressed.code == sf::Keyboard::Key::A || keyPressed.code == sf::Keyboard::Key::Left)
            body.ApplyForce(Vector2::LEFT * forceSpeed);
        if (keyPressed.code == sf::Keyboard::Key::S || keyPressed.code == sf::Keyboard::Key::Down)
            body.ApplyForce(Vector2::DOWN * forceSpeed);
        if (keyPressed.code == sf::Keyboard::Key::D || keyPressed.code == sf::Keyboard::Key::Right)
            body.ApplyForce(Vector2::RIGHT * forceSpeed);
    };
    window.handleEvents(OnKeyPressed);
}
