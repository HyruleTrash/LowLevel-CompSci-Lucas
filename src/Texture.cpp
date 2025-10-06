//
// Created by Lucas on 18/09/2025.
//

#include "Texture.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <stack>

Texture::Texture(const sf::RectangleShape& pixelTemplate, const sf::Vector2u& windowSize, Profiler& profiler) {
    this->profiler = &profiler;

    constexpr auto size = TextureConstants::getSize();
    constexpr auto sizeX = TextureConstants::getXSize();

    neighboursLookup.resize(size);
    std::generate(neighboursLookup.begin(), neighboursLookup.end(), []{
        return std::make_unique<PixelNeighbours>();
    });

    auto getRandomBool = [](std::uint64_t seed, size_t i) {
        std::mt19937_64 rng(seed ^ (i * 0x9E3779B97F4AC15));
        std::uniform_int_distribution<std::uint64_t> dist(0, 1);
        return static_cast<bool>(dist(rng));
    };

    const auto pixelSize = pixelTemplate.getSize();
    constexpr auto color = sf::Color(0, 255, 255, 150);

    // Create and fill renders, as well as setting the random position and states
    renders.resize(size);
    for (size_t i = 0; i < size; ++i) {
        renders[i] = std::make_unique<sf::RectangleShape>(pixelTemplate);

        const int column = i % sizeX;
        const int row = i / sizeX;

        const float x = static_cast<float>(column) * pixelSize.x;
        const float y = static_cast<float>(row) * pixelSize.y;
        renders[i]->setPosition({x, y});

        #ifndef NDEBUG // debug gradient for checking x, y positioning
        const std::uint8_t g = x / static_cast<float>(windowSize.x) * 255.0;
        const std::uint8_t b = y / static_cast<float>(windowSize.y) * 255.0;
        const auto colorDebug = sf::Color(0, g, b, 255);
        renders[i]->setFillColor(colorDebug);
        #endif

        renders[i]->setFillColor(color);

        // set cell state
        states[i] = getRandomBool(255, i);

        // set neighbour lookup table
        const bool canLeft = column != 0;
        const bool canRight = column != sizeX - 1;
        if (row != 0) { // canTop
            if (canLeft) {
                neighboursLookup[i]->neighbours.push_back(i - sizeX - 1);
                neighboursLookup[i - sizeX - 1]->neighbours.push_back(i);
            }
            if (canRight) {
                neighboursLookup[i]->neighbours.push_back(i - sizeX + 1);
                neighboursLookup[i - sizeX + 1]->neighbours.push_back(i);
            }
            neighboursLookup[i]->neighbours.push_back(i - sizeX);
            neighboursLookup[i - sizeX]->neighbours.push_back(i);
        }
        if (canLeft) {
            neighboursLookup[i]->neighbours.push_back(i - 1);
            neighboursLookup[i - 1]->neighbours.push_back(i);
        }
    }

    // calculate neighbours
    for (size_t i = 0; i < size; ++i) {
        std::for_each(neighboursLookup[i]->neighbours.begin(), neighboursLookup[i]->neighbours.end(), [&i, this](const auto& neighbourId) {
            if (states[neighbourId])
                neighboursLookup[i]->aliveNeighbourCount++;
        });
    }
}

void Texture::Update(sf::RenderWindow& window) const {
    CalculateNextState();
    Render(window);
}

void Texture::CalculateNextState() const {
    usedAndUpdated.reset();

    constexpr auto size = TextureConstants::getSize();
    constexpr auto sizeX = TextureConstants::getXSize();

    for (size_t i = 0; i < size; ++i) {
        const int liveNeighbors = neighboursLookup[i]->aliveNeighbourCount;

        if (usedAndUpdated[i])
            continue;
        if (states[i] && (liveNeighbors < 2 || liveNeighbors > 3)) {
            // underpopulation or overpopulation
            states[i] = false;
        }
        std::for_each(neighboursLookup[i]->neighbours.begin(), neighboursLookup[i]->neighbours.end(), [this](const size_t& neighbourId) {
            if (!usedAndUpdated[neighbourId]) {
                if (states[neighbourId] && (neighboursLookup[neighbourId]->aliveNeighbourCount < 2 || neighboursLookup[neighbourId]->aliveNeighbourCount > 3)) {
                    usedAndUpdated[neighbourId] = true;
                    states[neighbourId] = false; // underpopulation or overpopulation
                }
                else if (!states[neighbourId] && neighboursLookup[neighbourId]->aliveNeighbourCount == 3){ // crashed here
                    usedAndUpdated[neighbourId] = true;
                    states[neighbourId] = true; // reproduction
                }
            }
        });
    }

    for (size_t i = 0; i < size; ++i) {
        neighboursLookup[i]->aliveNeighbourCount = 0;
        std::for_each(neighboursLookup[i]->neighbours.begin(), neighboursLookup[i]->neighbours.end(), [&i, this](const auto& neighbourId) {
            if (states[neighbourId])
                neighboursLookup[i]->aliveNeighbourCount++;
        });
    }
}

void Texture::Render(sf::RenderWindow& window) const {
    for (size_t i = 0; i < TextureConstants::getSize(); ++i){
        if (states[i])
            window.draw(*renders[i]);
    }
}

std::ostream& operator<<(std::ostream& os, const Texture& texture) {
    os << TextureConstants::getXSize() << ", " << TextureConstants::getYSize() << std::endl;
    os << "pixels:" << std::endl;

    for (size_t i = 0; i < TextureConstants::getSize(); ++i) {
        os << "state = " << texture.states[i] << " " << i << std::endl;
        os << "AliveNeighbourCount = " << texture.neighboursLookup[i]->aliveNeighbourCount << std::endl;
        os << "neighbours [ " << std::endl;
        std::for_each(texture.neighboursLookup[i]->neighbours.begin(), texture.neighboursLookup[i]->neighbours.end(), [&i, &os, &texture](const auto& neighbourId) {
            os << texture.states[neighbourId] << " (" << neighbourId << ")" << std::endl;
        });
        os << "]" << std::endl;

    }

    return os;
}
