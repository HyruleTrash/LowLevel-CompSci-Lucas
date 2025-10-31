//
// Created by Lucas on 27/10/2025.
//

#include "ParticleSystem.h"

#include <algorithm>
#include <iostream>

#include "Particle.h"

size_t ParticleSystem::BATCH_SIZE = 1000;
std::chrono::time_point<std::chrono::system_clock> ParticleSystem::NOW;

ParticleSystem::ParticleSystem(sf::RenderWindow *win, const std::shared_ptr<Profiler>& profiler) : window(win),
    profiler(profiler), rng(std::random_device{}()),
    creationThread(BasicWaitingThread(std::bind(&ParticleSystem::SpawnParticles, this))),
    cleanupThread(BasicWaitingThread(std::bind(&ParticleSystem::CleanParticles, this))),
    renders(sf::PrimitiveType::Points, 0)
{
    const int numCores = std::thread::hardware_concurrency();
    const auto minThreadCount = std::max(numCores - 3, 1);
    // creationThreadPool = std::make_unique<WorkerThreadPool>(minThreadCount);
    updateThreadPool = std::make_unique<WorkerThreadPool>(minThreadCount);
    // cleanupThreadPool = std::make_unique<WorkerThreadPool>(minThreadCount);
}

void ParticleSystem::TriggerSpawnParticles(const int count, const sf::Vector2f origin) {
    spawnCount = count;
    spawnOrigin = origin;
    creationThread.Start();
}

void ParticleSystem::SpawnParticles() {
    const int count = spawnCount;
    const sf::Vector2f origin = spawnOrigin;

    std::uniform_real_distribution<float> angleDist(0, 2 * 3.14159f);
    std::uniform_real_distribution<float> speedDist(50, 200);
    std::uniform_int_distribution<int> colorDist(0, 255);
    std::uniform_real_distribution<float> lifeDist(1.0f, 5.0f);

    particleMutex.lock();
    const auto previousRendersCount = renders.getVertexCount();
    renders.resize(previousRendersCount + count);
    ReserveSpaceForNewParticles(count);
    particleMutex.unlock();

    for (size_t i = 0; i < count; ++i) {
        const float angle = angleDist(rng);
        const float speed = speedDist(rng);
        sf::Vector2f velocity(std::cos(angle) * speed, std::sin(angle) * speed);

        sf::Color color(colorDist(rng), colorDist(rng), colorDist(rng), 255);
        const float lifetime = lifeDist(rng);

        if (deadParticlePool.empty()) {
            CreateParticle(lifetime, origin, color, velocity, i, previousRendersCount);
        }
        else {
            ReUseParticle(lifetime, origin, color, velocity);
        }
    }
}

void ParticleSystem::ReserveSpaceForNewParticles(const int& count) {
    const auto nextCount = aliveFlags.size() + count;

    aliveFlags.reserve(nextCount);
    gravityFlags.reserve(nextCount);
    collisionFlags.reserve(nextCount);

    lifetimes.reserve(nextCount);
    maxLifetimes.reserve(nextCount);
    lastUpdateTimes.reserve(nextCount);

    positions.reserve(nextCount);
    velocities.reserve(nextCount);
    accelerations.reserve(nextCount);

    colors.reserve(nextCount);
}

void ParticleSystem::CreateParticle(const float& lifetime, const sf::Vector2f& origin, const sf::Color& color, const sf::Vector2f& velocity, const size_t& id, const size_t& previousRendersCount) {
    std::lock_guard<std::mutex> lock(particleMutex);
    auto& render = renders[previousRendersCount + id];
    render.color = color;
    render.position = origin;

    const auto& aliveFlag = aliveFlags.emplace_back(true);
    collisionFlags.push_back(true);
    gravityFlags.push_back(true);

    lifetimes.push_back(lifetime);
    maxLifetimes.push_back(lifetime);
    lastUpdateTimes.push_back(0.0f);

    positions.push_back(origin);
    accelerations.emplace_back(0, 98.1f); // Gravity
    velocities.push_back(velocity);

    colors.push_back(color);
    aliveParticleCount++;
}

/// Used for reconfiguring an existing particle
void ParticleSystem::ReUseParticle(const float& lifetime, const sf::Vector2f& origin, const sf::Color& color, const sf::Vector2f& velocity) {
    std::lock_guard<std::mutex> lock(particleMutex);
    const size_t id = deadParticlePool.back();
    lifetimes[id] = lifetime;
    maxLifetimes[id] = lifetime;

    positions[id] = origin;
    velocities[id] = velocity;
    colors[id] = color;

    aliveFlags[id] = true;
    deadParticlePool.pop_back();

    aliveParticleCount++;
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
    if (aliveFlags.empty())
        return;

    NOW = std::chrono::high_resolution_clock::now();

    if (aliveParticleCount != 0 && updateThreadPool->isIdle()) {
        const auto poolSize = updateThreadPool->GetPoolSize();
        const auto batchSize = aliveFlags.size() / poolSize;

        for (size_t i = 0; i < poolSize; ++i) {
            const size_t start = i * batchSize;
            const size_t end = std::min(start + batchSize, aliveFlags.size());

            updateThreadPool->enqueue([this, start, end, deltaTime]() {
                PROFILE(*profiler, "updatePhysics");
                for (size_t id = start; id < end; ++id) {
                    std::lock_guard<std::mutex> lock2(particleMutex);
                    if (aliveFlags.at(id)) {
                        Particle::UpdatePhysics(deltaTime, id, this);
                    }
                }
            });

            updateThreadPool->enqueue([this, start, end, deltaTime]() {
                PROFILE(*profiler, "updateColor");
                for (size_t id = start; id < end; ++id) {
                    std::lock_guard<std::mutex> lock2(particleMutex);
                    if (aliveFlags.at(id)) {
                        Particle::UpdateColor(deltaTime, id, this);
                    }
                }
            });

            updateThreadPool->enqueue([this, start, end, deltaTime]() {
                PROFILE(*profiler, "updateRender");
                for (size_t id = start; id < end; ++id) {
                    std::lock_guard<std::mutex> lock2(particleMutex);
                    if (aliveFlags.at(id)) {
                        Particle::UpdateRender(deltaTime, id, this);
                    }
                }
            });
        }
    }

    cleanupThread.Start();
}

void ParticleSystem::CleanParticles() {
    CleanDeadParticles();
    KillPendingRemovalParticles();
}

void ParticleSystem::CleanDeadParticles() {
    std::lock_guard<std::mutex> lock(particleMutex);
    if (deadParticlePool.empty())
        return;

    const auto now = std::chrono::duration<double>(NOW.time_since_epoch()).count();

    if (deadParticlesPoolSize == deadParticlePool.size() && now - lastUpdateTimes.at(deadParticlePool.back()) < PARTICLE_TIMEOUT)
        return;

    // PROFILE(*profiler, "CleanDeadParticles");

    const size_t poolOriginalSize = deadParticlePool.size();
    std::sort(deadParticlePool.begin(), deadParticlePool.end(),
              [this](const size_t a, const size_t b) {
                  return lastUpdateTimes.at(a) < lastUpdateTimes.at(b);
              });

    // Remove particles that have been dead for too long
    for (auto id = deadParticlePool.back(); !deadParticlePool.empty();) {
        // Check if particle still exists and is marked for removal, then check if it has been too long
        if (id < aliveFlags.size() && !aliveFlags[id] && (now - lastUpdateTimes.at(id)) > PARTICLE_TIMEOUT) {
            pendingRemovals.push_back(id);
            deadParticlePool.pop_back();
            id = deadParticlePool.back();
        } else {
            break;
        }
    }

    const size_t& poolSize = deadParticlePool.size();
    deadParticlesPoolSize = poolSize;
    if (poolOriginalSize != poolSize) {
        deadParticlePool.shrink_to_fit();
    }
}

void ParticleSystem::KillPendingRemovalParticles() {
    std::lock_guard<std::mutex> lock(particleMutex);
    if (pendingRemovals.empty())
        return;

    // PROFILE(*profiler, "KillPendingRemoval");

    const size_t size = std::min(BATCH_SIZE, pendingRemovals.size());
    int counter = 0;
    for (int i = 0; i < size && !pendingRemovals.empty(); ++i) {
        const size_t id = pendingRemovals.back();
        if (!aliveFlags[id]) {
            RemoveAt(id);
            pendingRemovals.pop_back();
            counter++;
        }
    }
    if (counter != 0) {
        if (aliveFlags.size() == 0)
            renders = sf::VertexArray(sf::PrimitiveType::Points, 0);
        else
            renders.resize(aliveFlags.size()); // bandaid shrink to fit
    }
    if (pendingRemovals.size() != 0)
        return;
    ShrinkToFit();
}

void ParticleSystem::ShrinkToFit() {
    pendingRemovals.shrink_to_fit();

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

    // PROFILE(*profiler, "removing particle");
    std::lock_guard<std::mutex> lock(particleMutex);

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

        renders[lastIndex].color = sf::Color::Transparent;
    }

    // Remove last element from all vectors
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

void ParticleSystem::SetDead(const size_t index) {
    aliveFlags.at(index) = false;
    deadParticlePool.push_back(index);
    renders[index].color = sf::Color::Transparent;
    aliveParticleCount--;
}

void ParticleSystem::Render() const {
    if (aliveParticleCount == 0)
        return;
    // PROFILE(*profiler, "render");
    window->draw(renders);
}
