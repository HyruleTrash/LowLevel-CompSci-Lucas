//
// Created by Lucas on 11/09/2025.
//

#ifndef LOW_LEVEL_SETUP_BALL_H
#define LOW_LEVEL_SETUP_BALL_H
#include "Entity.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

class Ball : public Entity {
public:
    Ball();
    ~Ball();
    void Update(sf::RenderWindow&, Profiler&) override;
public:
    sf::Color color = sf::Color::Green;
    float radius = 10;
private:
    sf::CircleShape shape;
};


#endif //LOW_LEVEL_SETUP_BALL_H