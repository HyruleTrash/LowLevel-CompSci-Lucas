//
// Created by Lucas on 11/09/2025.
//

#ifndef LOW_LEVEL_SETUP_BALL_H
#define LOW_LEVEL_SETUP_BALL_H
#include "Entity.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include "RigidBody.h"

class Ball : public Entity {
public:
    Ball();
    ~Ball()= default;
    void Render(sf::RenderWindow&, Profiler&, sf::Time& deltaTime) override;
    void Update(sf::RenderWindow&, Profiler&, sf::Time& deltaTime) override;
public:
    sf::Color color = sf::Color::Green;
    float radius = 40;
private:
    sf::CircleShape shape;
    RigidBody body;
};


#endif //LOW_LEVEL_SETUP_BALL_H