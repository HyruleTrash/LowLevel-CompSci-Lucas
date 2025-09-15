//
// Created by Lucas on 12/09/2025.
//

#include "RigidBody.h"

#include <iostream>

RigidBody::RigidBody() : RigidBody(1) {}

RigidBody::RigidBody(float mass) {
    this->mass = mass;
}

void RigidBody::ApplyForce(const sf::Vector2f& force) {
    this->force += force;
}

void RigidBody::ApplyImpulse(const sf::Vector2f& impulse) {
    momentum += impulse;
}

void RigidBody::Update(sf::RenderWindow& render_window, Profiler& profiler, sf::Time& deltaTime) {
    Component::Update(render_window, profiler, deltaTime);
    PROFILE(profiler, "RigidBody Update");
    momentum += force * deltaTime.asSeconds();
    velocity = momentum / mass;
    parent->position += velocity * deltaTime.asSeconds();
}
