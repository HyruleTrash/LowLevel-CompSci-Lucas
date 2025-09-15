//
// Created by Lucas on 12/09/2025.
//

#ifndef LOW_LEVEL_SETUP_RIGIDBODY_H
#define LOW_LEVEL_SETUP_RIGIDBODY_H
#include "Component.h"

class RigidBody : public Component {
public:
    RigidBody();
    RigidBody(float mass);
    void ApplyForce(const sf::Vector2f& force);
    void ApplyImpulse(const sf::Vector2f& impulse);
    void Update(sf::RenderWindow&, Profiler&, sf::Time& deltaTime) override;
public:
    float mass{1};
    sf::Vector2f velocity;
private:
    sf::Vector2f force;
    sf::Vector2f momentum;
};


#endif //LOW_LEVEL_SETUP_RIGIDBODY_H