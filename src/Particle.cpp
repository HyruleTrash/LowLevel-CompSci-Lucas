//
// Created by Lucas on 27/10/2025.
//

#include "Particle.h"

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Color col, float life): isDying(false), position(pos), lifetime(life), velocity(vel),
    hasGravity(true), color(col), effectType('n'), maxLifetime(life),
    isVisible(true), mass(1.0f), collisionEnabled(true), radius(2.0f) {

    // Shape
    shape = new sf::CircleShape(radius);
    shape->setFillColor(color);
    shape->setPosition(position);

    // Debug info
    static int counter = 0;
    debugName = "Particle_" + std::to_string(counter++);

    // Other data
    active = true;
    creationTime = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    id = counter;
    lastPosition = position;
    lastUpdateTime = creationTime;
    acceleration = sf::Vector2f(0, 98.1f); // Gravity
}

void Particle::update(float deltaTime) {
    if (!active) return;

    // Timing Update
    auto now = std::chrono::high_resolution_clock::now();
    double currentTime = std::chrono::duration<double>(now.time_since_epoch()).count();
    lastUpdateTime = currentTime;

    // Store last position
    lastPosition = position;

    // Physics calculations
    if (hasGravity) {
        velocity += acceleration * deltaTime;
    }

    position += velocity * deltaTime;

    // Color calculations
    float alpha = static_cast<float>(lifetime / maxLifetime);
    color.a = static_cast<uint8_t>(alpha * 255);

    // Update shape properties every frame
    shape->setPosition(position);
    shape->setFillColor(color);

    // Lifetime management
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        isDying = true;
        active = false;
    }

    // Bounds checking
    if (position.x < 0 || position.x > 800 || position.y < 0 || position.y > 800) {
        if (collisionEnabled) {
            velocity *= -0.8f;
            position.x = std::max(0.0f, std::min(800.0f, position.x));
            position.y = std::max(0.0f, std::min(800.0f, position.y));
        }
    }
}

void Particle::render(sf::RenderWindow &window) {
    if (isVisible && active) {
        window.draw(*shape);
    }
}

bool Particle::isAlive() const {
    return active && !isDying;
}
