//
// Created by Lucas on 27/10/2025.
//

#include "ParticleSystem.h"

#include <iostream>

#include "Particle.h"

ParticleSystem::ParticleSystem(sf::RenderWindow *win): window(win), rng(std::random_device{}()) {
    // Pre-allocation
    particles.reserve(100);

    renders.reserve(100);

    aliveFlags.reserve(100);
    gravityFlags.reserve(100);
    collisionFlags.reserve(100);

    lifetimes.reserve(100);
    maxLifetimes.reserve(100);

    positions.reserve(100);
    lastPositions.reserve(100);
    velocities.reserve(100);
    accelerations.reserve(100);

    colors.reserve(100);
}

void ParticleSystem::spawnParticles(const int count, const sf::Vector2f origin) {
    std::uniform_real_distribution<float> angleDist(0, 2 * 3.14159f);
    std::uniform_real_distribution<float> speedDist(50, 200);
    std::uniform_int_distribution<int> colorDist(0, 255);
    std::uniform_real_distribution<float> lifeDist(1.0f, 5.0f);

    for (size_t i = 0; i < count; ++i) {
        const float angle = angleDist(rng);
        const float speed = speedDist(rng);
        sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);

        sf::Color color(colorDist(rng), colorDist(rng), colorDist(rng), 255);
        float lifetime = lifeDist(rng);

        // Create particle
        renders.push_back(sf::CircleShape(2.0f));

        aliveFlags.push_back(true);
        collisionFlags.push_back(true);
        gravityFlags.push_back(true);

        lifetimes.push_back(lifetime);
        maxLifetimes.push_back(lifetime);

        positions.push_back(origin);
        lastPositions.push_back(origin);
        accelerations.push_back(sf::Vector2f(0, 98.1f)); // Gravity
        velocities.push_back(velocity);

        colors.push_back(color);

        auto particle = std::make_unique<Particle>(i, this);

        // Store in containers
        particles.push_back(std::move(particle));
    }
}

void ParticleSystem::update(const float deltaTime) {
    for (size_t i = 0; i < renders.size(); ++i) {
        if (aliveFlags.at(i))
            particles[i]->update(deltaTime, i, this);
    }
}

void ParticleSystem::render() const {
    // Render all particles
    for (size_t i = 0; i < renders.size(); ++i) {
        if (aliveFlags.at(i))
            window->draw(renders.at(i));
    }
}

size_t ParticleSystem::getParticleCount() const {
    return aliveFlags.size();
}

bool ParticleSystem::isAlive(const size_t index) const {
    return aliveFlags.at(index);
}
