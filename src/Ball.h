//
// Created by Lucas on 06/10/2025.
//

#ifndef LOW_LEVEL_SETUP_BALL_H
#define LOW_LEVEL_SETUP_BALL_H
#include "SFML/Graphics/CircleShape.hpp"


class Ball {
public:
    Ball(const float x, const float y, const float radius, const sf::Color color, const float vx, const float vy) {
        shape.setRadius(radius);
        shape.setPosition(sf::Vector2f(x, y));
        shape.setFillColor(color);
        shape.setOrigin(sf::Vector2f(radius, radius)); // Center origin
        velocity = sf::Vector2f(vx, vy);
    }
    std::tuple<float, float> GetPosition() const;
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    std::tuple<int, int> hashPosition;
};


#endif //LOW_LEVEL_SETUP_BALL_H