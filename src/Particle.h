//
// Created by Lucas on 27/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H
#define PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H
#include <random>
#include <SFML/Graphics.hpp>

#include "GameObject.h"

class Particle : public GameObject {
public:
    Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Color col, float life);
    ~Particle() override = default;

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool isAlive() const override;

    // TODO: Expensive getter functions called frequently
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getVelocity() const { return velocity; }
    std::string getDebugName() const { return debugName; }
    double getCreationTime() const { return creationTime; }

private:
    sf::CircleShape *shape;
    bool isDying;
    bool isVisible;
    bool hasGravity;
    bool collisionEnabled;
    char effectType;
    float radius;
    float mass;
    double lifetime;
    double maxLifetime;
    double lastUpdateTime;
    std::string debugName;
    sf::Vector2f position;
    sf::Vector2f lastPosition;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Color color;
};

#endif //PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H