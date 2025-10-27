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
    bool isDying;
    sf::Vector2f position;
    double lifetime;
    sf::Vector2f velocity;
    bool hasGravity;
    sf::Color color;
    char effectType;
    sf::Vector2f acceleration;
    double maxLifetime;
    sf::CircleShape *shape;
    bool isVisible;
    float mass;
    sf::Vector2f lastPosition;
    std::string debugName;
    double lastUpdateTime;
    bool collisionEnabled;
    float radius;
};

#endif //PARTICLE_SYSTEM_ASSIGNMENT_PARTICLE_H