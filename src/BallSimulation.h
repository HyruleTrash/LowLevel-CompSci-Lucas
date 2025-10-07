//
// Created by Lucas on 06/10/2025.
//

#ifndef LOW_LEVEL_SETUP_BALLSIMULATION_H
#define LOW_LEVEL_SETUP_BALLSIMULATION_H
#include <random>
#include <vector>

#include "Ball.h"
#include "Profiler.hpp"
#include "SpatialHash.h"
#include "SFML/Graphics/RenderWindow.hpp"


class BallSimulation {
private:
    std::shared_ptr<Profiler> profiler;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> posDist;
    std::uniform_real_distribution<float> velDist;
    std::uniform_int_distribution<int> colorDist;
    std::uniform_real_distribution<float> radiusDist;
    int cellSize{128};
    std::unique_ptr<SpatialHash> spatialHash;
    std::vector<std::shared_ptr<Ball>> balls;
public:
    BallSimulation(const std::shared_ptr<Profiler>&);
    void UpdateBalls(const sf::Vector2u& windowSize, float deltaTime) const;
    void drawBalls( sf::RenderWindow& window ) const;
};


#endif //LOW_LEVEL_SETUP_BALLSIMULATION_H