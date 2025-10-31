//
// Created by Lucas on 27/10/2025.
//

#include "Particle.h"

#include <iostream>

#include "particleSystem.h"

void Particle::UpdatePhysics(const float deltaTime, const size_t id, ParticleSystem* particleSystem) {
    // PROFILE(*particleSystem->profiler, "Update particles");

    auto& lastUpdateTime = particleSystem->lastUpdateTimes.at(id);
    auto& position = particleSystem->positions.at(id);
    const auto& acceleration = particleSystem->accelerations.at(id);
    auto& velocity = particleSystem->velocities.at(id);

    // update last update timestamp
    lastUpdateTime = std::chrono::duration<double>(ParticleSystem::NOW.time_since_epoch()).count();

    // Physics calculations
    if (particleSystem->gravityFlags.at(id)) {
        velocity += acceleration * deltaTime;
    }

    position += velocity * deltaTime;

    // Bounds checking
    if (position.x < 0 || position.x > 800 || position.y < 0 || position.y > 800) {
        if (particleSystem->collisionFlags.at(id)) {
            velocity *= -0.8f;
            position.x = std::max(0.0f, std::min(800.0f, position.x));
            position.y = std::max(0.0f, std::min(800.0f, position.y));
        }
    }
}

void Particle::UpdateColor(const float deltaTime, const size_t id, ParticleSystem* particleSystem) {
    auto& lifetime = particleSystem->lifetimes.at(id);

    // Color calculations
    auto& color = particleSystem->colors.at(id);
    const auto& maxLifetime = particleSystem->maxLifetimes.at(id);
    const auto alpha = static_cast<float>(lifetime / maxLifetime);
    color.a = static_cast<uint8_t>(alpha * 255);

    // Lifetime management
    lifetime -= deltaTime;
    if (lifetime <= 0) {
        particleSystem->SetDead(id);
        return;
    }
}

void Particle::UpdateRender(const float deltaTime, const size_t id, ParticleSystem* particleSystem) {
    const auto& position = particleSystem->positions.at(id);
    const auto& color = particleSystem->colors.at(id);
    auto& render = particleSystem->renders[id];
    render.position = position;
    render.color = color;
}