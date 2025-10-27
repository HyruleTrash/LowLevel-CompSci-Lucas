//
// Created by Lucas on 27/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H
#define PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H
#include <random>
#include <SFML/Graphics.hpp>

class ParticleSystem;

class Particle {
public:
    Particle(const std::shared_ptr<std::vector<sf::Vector2f>> &pos,
            const std::shared_ptr<std::vector<sf::Vector2f>> &vel,
            const std::shared_ptr<std::vector<sf::Color>> &col,
            float life,
            const std::shared_ptr<std::vector<bool>> &aliveFlag,
            const std::shared_ptr<std::vector<sf::CircleShape>> &render);
    ~Particle() = default;

    void update(float deltaTime);

private:
    std::shared_ptr<std::vector<sf::CircleShape>> renders;
    std::shared_ptr<std::vector<bool>> aliveFlags;
    std::shared_ptr<std::vector<sf::Vector2f>> positions;
    std::shared_ptr<std::vector<sf::Vector2f>> velocities;
    std::shared_ptr<std::vector<sf::Color>> colors;

    bool isVisible;
    bool hasGravity;
    bool collisionEnabled;
    size_t id; // todo: set this
    double lifetime;
    double maxLifetime;
    double lastUpdateTime;
    double creationTime;
    sf::Vector2f lastPosition;
    sf::Vector2f acceleration;
};

#endif //PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H