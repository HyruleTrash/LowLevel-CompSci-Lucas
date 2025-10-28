//
// Created by Lucas on 27/10/2025.
//

#include "ParticleSystem.h"

#include <algorithm>
#include <iostream>

#include "Particle.h"

ParticleSystem::ParticleSystem(sf::RenderWindow *win, const std::shared_ptr<Profiler>& profiler): window(win), profiler(profiler), rng(std::random_device{}()) {
    renders = sf::VertexArray(sf::PrimitiveType::Points, 0);
}

void ParticleSystem::SpawnParticles(const int count, const sf::Vector2f origin) {
    std::uniform_real_distribution<float> angleDist(0, 2 * 3.14159f);
    std::uniform_real_distribution<float> speedDist(50, 200);
    std::uniform_int_distribution<int> colorDist(0, 255);
    std::uniform_real_distribution<float> lifeDist(1.0f, 5.0f);

    const auto previousRendersCount = renders.getVertexCount();
    renders.resize(previousRendersCount + count);
    for (size_t i = 0; i < count; ++i) {
        const float angle = angleDist(rng);
        const float speed = speedDist(rng);
        sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);

        sf::Color color(colorDist(rng), colorDist(rng), colorDist(rng), 255);
        const float lifetime = lifeDist(rng);

        if (deadParticlePool.empty()) {
            // Create particle
            auto& render = renders[previousRendersCount + i];
            render.color = color;
            render.position = origin;

            aliveFlags.push_back(true);
            collisionFlags.push_back(true);
            gravityFlags.push_back(true);

            lifetimes.push_back(lifetime);
            maxLifetimes.push_back(lifetime);
            lastUpdateTimes.push_back(0.0f);

            positions.push_back(origin);
            accelerations.emplace_back(0, 98.1f); // Gravity
            velocities.push_back(velocity);

            colors.push_back(color);
        }
        else {
            // reconfigure particle
            const size_t id = deadParticlePool.back();
            lifetimes[id] = lifetime;
            maxLifetimes[id] = lifetime;

            positions[id] = origin;
            velocities[id] = velocity;
            colors[id] = color;

            aliveFlags[id] = true;
            deadParticlePool.pop_back();
        }
    }
}

void ParticleSystem::SwapBool(std::_Bit_reference x, std::_Bit_reference y) {
    // First XOR operation: Store combined state in x
    x = x ^ y;

    // Second XOR operation: Extract original value of x into y
    y = x ^ y;

    // Third XOR operation: Extract original value of y into x
    x = x ^ y;
}

void ParticleSystem::Update(const float deltaTime) {
    if (breaker){
        std::cout << "breaker" << std::endl;
        return;
    }

    if (aliveFlags.empty())
        return;

    for (size_t i = 0; i < aliveFlags.size(); ++i) {
        PROFILE(*profiler, "Update particles");
        if (aliveFlags.at(i))
            Particle::update(deltaTime, i, this);
    }

    CleanDeadParticles();
    KillPendingRemovalParticles();
}

void ParticleSystem::CleanDeadParticles() {
    PROFILE(*profiler, "CleanDeadParticles");
    const auto now = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    // Remove particles that have been dead for too long
    for (auto it = deadParticlePool.begin(); it != deadParticlePool.end();) {
        const size_t id = *it;

        // Check if particle still exists and is marked for removal, then check if it has been too long
        if (id < aliveFlags.size() && !aliveFlags[id] && (now - lastUpdateTimes.at(id)) > 10) {
            pendingRemovals.push_back(id);
            it = deadParticlePool.erase(it);
        } else {
            ++it;
        }
    }

    deadParticlePool.shrink_to_fit();
}

void ParticleSystem::KillPendingRemovalParticles() {
    PROFILE(*profiler, "KillPendingRemoval");
    const size_t BATCH_SIZE = std::min(static_cast<size_t>(100), pendingRemovals.size());
    for (int i = 0; i < BATCH_SIZE && !pendingRemovals.empty(); ++i) {
        const size_t id = pendingRemovals.back();
        if (!aliveFlags[id]) {
            RemoveAt(id);
            pendingRemovals.pop_back();
        }
    }

    if (pendingRemovals.size() != 0)
        return;
    pendingRemovals.shrink_to_fit();

    renders.resize(aliveFlags.size());
    aliveFlags.shrink_to_fit();
    gravityFlags.shrink_to_fit();
    collisionFlags.shrink_to_fit();

    lifetimes.shrink_to_fit();
    maxLifetimes.shrink_to_fit();
    lastUpdateTimes.shrink_to_fit();

    positions.shrink_to_fit();
    velocities.shrink_to_fit();
    accelerations.shrink_to_fit();

    colors.shrink_to_fit();
}

void ParticleSystem::RemoveAt(const size_t index) {
    if (index >= aliveFlags.size()) {
        return;
    }

    // Swap element with last element
    if (const size_t lastIndex = aliveFlags.size() - 1; index != lastIndex) {
        std::swap(renders[index], renders[lastIndex]);

        SwapBool(aliveFlags[index], aliveFlags[lastIndex]);
        SwapBool(gravityFlags[index], gravityFlags[lastIndex]);
        SwapBool(collisionFlags[index], collisionFlags[lastIndex]);

        std::swap(lifetimes[index], lifetimes[lastIndex]);
        std::swap(maxLifetimes[index], maxLifetimes[lastIndex]);
        std::swap(lastUpdateTimes[index], lastUpdateTimes[lastIndex]);

        std::swap(positions[index], positions[lastIndex]);
        std::swap(velocities[index], velocities[lastIndex]);
        std::swap(accelerations[index], accelerations[lastIndex]);

        std::swap(colors[index], colors[lastIndex]);

        // Update IDs in deadParticlePool & pendingRemovals
        for (size_t& idRef : deadParticlePool) {
            if (idRef == index) {
                idRef = lastIndex;
            } else if (idRef == lastIndex) {
                idRef = index;
            }
        }
        for (size_t& idRef : pendingRemovals) {
            if (idRef == index) {
                idRef = lastIndex;
            } else if (idRef == lastIndex) {
                idRef = index;
            }
        }
    }

    // Remove last element from all vectors
    renders.resize(renders.getVertexCount() - 1);
    aliveFlags.pop_back();
    gravityFlags.pop_back();
    collisionFlags.pop_back();

    lifetimes.pop_back();
    maxLifetimes.pop_back();
    lastUpdateTimes.pop_back();

    positions.pop_back();
    velocities.pop_back();
    accelerations.pop_back();

    colors.pop_back();
}

void ParticleSystem::Render() {
    for (size_t i = 0; i < aliveFlags.size(); ++i) {
        if (!aliveFlags[i])
            continue;

        auto& render = renders[i];
        if (render.position != positions[i] || render.color != colors[i]) {
            render.position = positions[i];
            render.color = colors[i];
        }
    }

    window->draw(renders);
}