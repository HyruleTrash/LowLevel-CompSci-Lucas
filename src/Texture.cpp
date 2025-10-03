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

    // Set size of used collection classes
    renders.resize(TEX_SIZE);
    neighboursLookup.resize(TEX_SIZE);

    auto getRandomBool = [](std::uint64_t seed, size_t i) {
        std::mt19937_64 rng(seed ^ (i * 0x9E3779B97F4AC15));
        std::uniform_int_distribution<std::uint64_t> dist(0, 1);
        return static_cast<bool>(dist(rng));
    };

    const auto pixelSize = pixelTemplate.getSize();
    constexpr auto color = sf::Color(0, 255, 255, 150);

    // Create and fill renders, as well as setting the random position and states
    for (size_t i = 0; i < TEX_SIZE; ++i) {
        renders[i] = std::make_unique<sf::RectangleShape>(pixelTemplate);
        neighboursLookup[i] = std::make_unique<PixelNeighbours>();

        const int modX = i % TEX_SIZE_X;
        const int row = i / TEX_SIZE_X;

        const float x = static_cast<float>(modX) * pixelSize.x;
        const float y = static_cast<float>(row) * pixelSize.y;
        renders[i]->setPosition({x, y});

        #ifndef NDEBUG
        const std::uint8_t g = x / static_cast<float>(windowSize.x) * 255.0;
        const std::uint8_t b = y / static_cast<float>(windowSize.y) * 255.0;
        const auto colorDebug = sf::Color(0, g, b, 255);
        renders[i]->setFillColor(colorDebug);
        #endif

        renders[i]->setFillColor(color);

        states[i] = getRandomBool(255, i);
    }

    // Setup neighbour lookup table
    for (size_t i = 0; i < TEX_SIZE; ++i) {
        const int modX = i % TEX_SIZE_X;
        const int row = i / TEX_SIZE_X;

        const bool canLeft = modX != 0;
        const bool canRight = modX != TEX_SIZE_X - 1;
        if (row != 0) { // canTop
            if (canLeft) {
                neighboursLookup[i]->neighbours.push_back({-1,-1});
                neighboursLookup[i - TEX_SIZE_X - 1]->neighbours.push_back({1,1});
            }
            if (canRight) {
                neighboursLookup[i]->neighbours.push_back({1,-1});
                neighboursLookup[i - TEX_SIZE_X + 1]->neighbours.push_back({-1,1});
            }
            neighboursLookup[i]->neighbours.push_back({0,-1});
            neighboursLookup[i - TEX_SIZE_X]->neighbours.push_back({0,1});
        }
        if (canLeft) {
            neighboursLookup[i]->neighbours.push_back({-1,0});
            neighboursLookup[i - 1]->neighbours.push_back({1,0});
        }
    }

    // calculate neighbours
    for (size_t i = 0; i < TEX_SIZE; ++i) {
        std::for_each(neighboursLookup[i]->neighbours.begin(), neighboursLookup[i]->neighbours.end(), [&i, this](const auto& neighbourPosition) {
            const auto [x, y] = neighbourPosition;
            const auto neighbourId = static_cast<long>(i) + x + y * TEX_SIZE_X;
            if (states[neighbourId])
                neighboursLookup[i]->aliveNeighbourCount++;
        });
    }

    // std::cout << *this << std::endl;
}

void Texture::Update(sf::RenderWindow& window) const {
    CalculateNextState();
    Render(window);
}

void Texture::CalculateNextState() const {
    PROFILE(*profiler, "calculate pixel");
    for (size_t i = 0; i < TEX_SIZE; ++i) {
        PROFILE(*profiler, "pixel state logic");
        const int liveNeighbors = neighboursLookup[i]->aliveNeighbourCount;

        if (states[i] && (liveNeighbors < 2 || liveNeighbors > 3)) {
            // underpopulation or overpopulation
            states[i] = false;
        }else if (liveNeighbors == 3) {
            // reproduction
            states[i] = true;
        }
    }

    for (size_t i = 0; i < TEX_SIZE; ++i) {
        PROFILE(*profiler, "neighbour counting logic");
        neighboursLookup[i]->aliveNeighbourCount = 0;
        std::for_each(neighboursLookup[i]->neighbours.begin(), neighboursLookup[i]->neighbours.end(), [&i, this](const auto& neighbourPosition) {
            const auto [x, y] = neighbourPosition;
            const auto neighbourId = static_cast<long>(i) + x + y * TEX_SIZE_X;
            if (states[neighbourId])
                neighboursLookup[i]->aliveNeighbourCount++;
        });
    }
}

void Texture::Render(sf::RenderWindow& window) const {
    for (size_t i = 0; i < TEX_SIZE; ++i){
        PROFILE(*profiler, "pixel render");
        if (states[i])
            window.draw(*renders[i]);
    }
}

std::ostream& operator<<(std::ostream& os, const Texture& texture) {
    os << TEX_SIZE_X << ", " << TEX_SIZE_Y << std::endl;
    os << "pixels:" << std::endl;

    // std::stringstream afterPrint{};
    for (size_t i = 0; i < TEX_SIZE; ++i) {
        // if (i % texture.size.x == 0) {
        //     os << std::endl;
        // }
        // os << pixel->alive << " ";

        // afterPrint << std::endl << "Pixel:" << std::endl;
        os << "state = " << texture.states[i] << " " << i << std::endl;
        os << "AliveNeighbourCount = " << texture.neighboursLookup[i]->aliveNeighbourCount << std::endl;
        // afterPrint << "render = " << pixel->render.get() << std::endl;
        // if (pixel->render != nullptr) {
        //     afterPrint << "render [ " << std::endl;
        //     auto color = pixel->render->getFillColor();
        //     afterPrint << "color = " << color.r << ", " << color.g << ", " << color.b << std::endl;
        //     auto pos = pixel->render->getPosition();
        //     afterPrint << "position = " << pos.x << ", " << pos.y << std::endl;
        //     auto size = pixel->render->getSize();
        //     afterPrint << "size = " << size.x << ", " << size.y << std::endl;
        //     afterPrint << "]" << std::endl;
        // }
        os << "neighbours [ " << std::endl;
        std::for_each(texture.neighboursLookup[i]->neighbours.begin(), texture.neighboursLookup[i]->neighbours.end(), [&i, &os, &texture](const auto& neighbourPosition) {
            const auto [x, y] = neighbourPosition;
            os << "[" << static_cast<int>(x) << ", " << static_cast<int>(y) << "] = ";
            const auto neighbourId = static_cast<long>(i) + x + y * TEX_SIZE_X;
            os << texture.states[neighbourId] << " " << neighbourId << std::endl;
        });
        os << "]" << std::endl;

    }
    // os << std::endl << afterPrint.str() << std::endl;

    return os;
}
