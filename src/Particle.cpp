//
// Created by Lucas on 27/10/2025.
//

#include "Particle.h"

#include <iostream>

#include "ParticleSystem.h"

Particle::Particle(const std::shared_ptr<std::vector<sf::Vector2f>> &pos,
    const std::shared_ptr<std::vector<sf::Vector2f>> &vel, const std::shared_ptr<std::vector<sf::Color>> &col, const float life,
    const std::shared_ptr<std::vector<bool>> &aliveFlag, const std::shared_ptr<std::vector<sf::CircleShape>> &render):
    renders(render), aliveFlags(aliveFlag), positions(pos), velocities(vel), colors(col), isVisible(true),
    hasGravity(true), collisionEnabled(true), lifetime(life), maxLifetime(life)
{
    static size_t counter = 0; // to calculate the id

    // Other data
    creationTime = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    id = counter++;
    lastUpdateTime = creationTime;

    // Shape
    const auto& position = positions->at(id);
    renders->at(id).setFillColor(colors->at(id));
    renders->at(id).setPosition(position);

    lastPosition = position;
    acceleration = sf::Vector2f(0, 98.1f); // Gravity
}

void Particle::update(const float deltaTime) {
    // Timing Update
    const auto now = std::chrono::high_resolution_clock::now();
    const double currentTime = std::chrono::duration<double>(now.time_since_epoch()).count();
    lastUpdateTime = currentTime;

    // Store last position
    lastPosition = positions->at(id);

    // Physics calculations
    auto& position = positions->at(id);
    auto& velocity = velocities->at(id);
    if (hasGravity) {
        velocity += acceleration * deltaTime;
    }

    positions->at(id) += velocity * deltaTime;

    // Color calculations
    auto& color = colors->at(id);
    const auto alpha = static_cast<float>(lifetime / maxLifetime);
    color.a = static_cast<uint8_t>(alpha * 255);

    // Update shape properties every frame
    renders->at(id).setFillColor(color);
    renders->at(id).setPosition(position);

    // Lifetime management
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        aliveFlags->at(id) = false;
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
