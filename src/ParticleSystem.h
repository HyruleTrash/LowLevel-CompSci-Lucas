//
// Created by Lucas on 27/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H
#define PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H
#include <memory>
#include <random>
#include <SFML/Graphics.hpp>

#include "Particle.h"

class ParticleSystem {
public:
    ParticleSystem(sf::RenderWindow* win);

    void spawnParticles(int count, sf::Vector2f origin);

    void update(float deltaTime) const;
    void render() const;

    size_t getParticleCount() const;
    bool isAlive(size_t index) const;

private:
    sf::RenderWindow* window;
    std::mt19937 rng;

    // Particle Container
    std::vector<std::unique_ptr<Particle>> particles;
    std::shared_ptr<std::vector<sf::CircleShape>> renders;

    // Separate vectors per important property
    std::shared_ptr<std::vector<bool>> aliveFlags;
    std::shared_ptr<std::vector<sf::Vector2f>> positions;
    std::shared_ptr<std::vector<sf::Vector2f>> velocities;
    std::shared_ptr<std::vector<sf::Color>> colors;
};


#endif //PARTICLE_SYSTEM_ASSIGNMENT_PARTICLESYSTEM_H