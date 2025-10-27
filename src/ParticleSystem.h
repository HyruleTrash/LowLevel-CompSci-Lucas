//
// Created by Lucas on 27/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H
#define PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H
#include <memory>
#include <random>
#include <SFML/Graphics.hpp>

#include "GameObject.h"

class ParticleSystem {
public:
    ParticleSystem(sf::RenderWindow* win);

    void spawnParticles(int count, sf::Vector2f origin);

    void update(float deltaTime);
    void render() const;

    size_t getParticleCount() const;

private:
    // Particle Container
    std::vector<std::unique_ptr<GameObject>> particles;

    // Separate vectors per property
    std::vector<sf::Vector2f> positions;
    std::vector<sf::Vector2f> velocities;
    std::vector<sf::Color> colors;
    std::vector<bool> aliveFlags;

    sf::RenderWindow* window;
    std::mt19937 rng;
};


#endif //PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H