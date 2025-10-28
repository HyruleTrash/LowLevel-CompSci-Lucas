//
// Created by Lucas on 27/10/2025.
//

#include "Particle.h"

#include <iostream>

#include "particleSystem.h"

void Particle::update(const float deltaTime, const size_t id, ParticleSystem* particleSystem) {
    auto& lastUpdateTime = particleSystem->lastUpdateTimes.at(id);
    auto& position = particleSystem->positions.at(id);
    const auto& acceleration = particleSystem->accelerations.at(id);
    auto& velocity = particleSystem->velocities.at(id);
    auto& lifetime = particleSystem->lifetimes.at(id);

    // update last update timestamp
    const auto now = std::chrono::high_resolution_clock::now();
    lastUpdateTime = std::chrono::duration<double>(now.time_since_epoch()).count();

    // Physics calculations
    if (particleSystem->gravityFlags.at(id)) {
        velocity += acceleration * deltaTime;
    }

    position += velocity * deltaTime;

    // Color calculations
    auto& color = particleSystem->colors.at(id);
    const auto& maxLifetime = particleSystem->maxLifetimes.at(id);
    const auto alpha = static_cast<float>(lifetime / maxLifetime);
    color.a = static_cast<uint8_t>(alpha * 255);

    // Update shape properties every frame
    auto& render = particleSystem->renders.at(id);
    render.setFillColor(color);
    render.setPosition(position);

    // Lifetime management
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        particleSystem->aliveFlags.at(id) = false;
        particleSystem->deadParticlePool.push_back(id);
    }

    // Bounds checking
    if (position.x < 0 || position.x > 800 || position.y < 0 || position.y > 800) {
        if (particleSystem->collisionFlags.at(id)) {
            velocity *= -0.8f;
            position.x = std::max(0.0f, std::min(800.0f, position.x));
            position.y = std::max(0.0f, std::min(800.0f, position.y));
        }
    }
}
