//
// Created by Lucas on 06/10/2025.
//

#ifndef LOW_LEVEL_SETUP_BALLSIMULATION_H
#define LOW_LEVEL_SETUP_BALLSIMULATION_H
#include <random>
#include <vector>

#include "Ball.h"
#include "Profiler.hpp"
#include "SFML/Graphics/RenderWindow.hpp"


class BallSimulation {
private:
    std::vector<Ball> balls;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> posDist;
    std::uniform_real_distribution<float> velDist;
    std::uniform_int_distribution<int> colorDist;
    std::uniform_real_distribution<float> radiusDist;
    std::shared_ptr<Profiler> profiler;
public:
    BallSimulation(const std::shared_ptr<Profiler>&);
    void updateBalls(const sf::Vector2u& windowSize, float deltaTime);
    void drawBalls( sf::RenderWindow& window ) const;
};


#endif //LOW_LEVEL_SETUP_BALLSIMULATION_H