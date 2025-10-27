//
// Created by Lucas on 27/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H
#define PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H
#include <memory>
#include <random>

class ParticleSystem;

class Particle {
public:
    Particle(size_t id, ParticleSystem* particleSystem);
    ~Particle() = default;

    static void update(float deltaTime, size_t id, ParticleSystem* particleSystem);
};

#endif //PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H