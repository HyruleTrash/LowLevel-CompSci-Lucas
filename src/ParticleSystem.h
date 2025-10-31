//
// Created by Lucas on 27/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H
#define PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H
#include <algorithm>
#include <memory>
#include <random>
#include <stack>
#include <SFML/Graphics.hpp>

#include "BasicWaitingThread.h"
#include "Particle.h"
#include "Profiler.hpp"
#include "WorkerThreadPool.h"

class ParticleSystem {
public:
    ParticleSystem(sf::RenderWindow* win, const std::shared_ptr<Profiler>& profiler);

    void TriggerSpawnParticles(int count, sf::Vector2f origin);
    void ReserveSpaceForNewParticles(const int &count);

    void CreateParticle(const float &lifetime, const sf::Vector2f &origin, const sf::Color &color,
                        const sf::Vector2f &velocity, const size_t &id, const size_t &previousRendersCount);
    void ReUseParticle(const float &lifetime, const sf::Vector2f &origin, const sf::Color &color,
                       const sf::Vector2f &velocity);

    static void SwapBool(std::_Bit_reference x, std::_Bit_reference y);

    void Update(float deltaTime);

    void CleanParticles();

    void Render() const;

    void CleanDeadParticles();
    void KillPendingRemovalParticles();

    void ShrinkToFit();

    void RemoveAt(size_t index);
    void SetDead(size_t index);
public:
    void SpawnParticles();

private:
    sf::RenderWindow* window;
    std::mt19937 rng;
public:
    static constexpr int PARTICLE_TIMEOUT{60};
    static size_t BATCH_SIZE;
    static std::chrono::time_point<std::chrono::system_clock> NOW;
    std::shared_ptr<Profiler> profiler;
    int spawnCount;
    sf::Vector2f spawnOrigin;
    size_t aliveParticleCount{0};
    size_t deadParticlesPoolSize{0};
    std::mutex particleMutex;
    BasicWaitingThread creationThread;
    BasicWaitingThread cleanupThread;
    // std::unique_ptr<WorkerThreadPool> creationThreadPool;
    // std::unique_ptr<WorkerThreadPool> cleanupThreadPool;
    std::unique_ptr<WorkerThreadPool> updateThreadPool;
    std::vector<size_t> deadParticlePool;
    std::vector<size_t> pendingRemovals;
    // Separate vectors per important property
    sf::VertexArray renders;
    std::vector<bool> aliveFlags, gravityFlags, collisionFlags;
    std::vector<double> lifetimes, maxLifetimes, lastUpdateTimes;
    std::vector<sf::Vector2f> positions, velocities, accelerations;
    std::vector<sf::Color> colors;
};


#endif //PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H