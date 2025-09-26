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

Texture::Texture(const sf::RectangleShape& pixelTemplate, const sf::Vector2u& texSize, const sf::Vector2u& windowSize, Profiler& profiler)  : name("Texture"), size(texSize.x, texSize.y){
    this->profiler = &profiler;

    pixels.resize(size.x * size.y);
    auto pixelSize = pixelTemplate.getSize();

    auto getRandomBool = [](std::uint64_t seed, size_t i) {
        std::mt19937_64 rng(seed ^ (i * 0x9E3779B97F4AC15));
        std::uniform_int_distribution<std::uint64_t> dist(0, 1);
        return static_cast<bool>(dist(rng));
    };

    for (size_t i = 0; i < pixels.size(); ++i) {
        const auto pixel = new Pixel();
        pixel->render = std::make_unique<sf::RectangleShape>(pixelTemplate);

        const int modX = i % size.x;
        const int row = i / size.x;

        const float x = static_cast<float>(modX) * pixelSize.x;
        const float y = static_cast<float>(row) * pixelSize.y;
        pixel->render->setPosition({x, y});

        #ifndef NDEBUG
        const std::uint8_t g = x / static_cast<float>(windowSize.x) * 255.0;
        const std::uint8_t b = y / static_cast<float>(windowSize.y) * 255.0;
        const auto colorDebug = sf::Color(0, g, b, 255);
        pixel->render->setFillColor(colorDebug);
        #endif

        auto color = sf::Color::Cyan;
        color.a = 150;
        pixel->render->setFillColor(color);

        pixel->alive = getRandomBool(255, i);

        pixels[i] = pixel;
    }

    for (size_t i = 0; i < pixels.size(); ++i) {
        const auto pixel = pixels[i];
        const int modX = i % size.x;
        const int row = i / size.x;

        const bool canLeft = modX != 0;
        const bool canRight = modX != size.x - 1;
        const bool canTop = row != 0;
        // const bool canBottom = row != size.y - 1;
        if (canTop) {
            if (canLeft) {
                pixel->neighbours.push_back(pixels[i - size.x - 1]);
                pixels[i - size.x - 1]->neighbours.push_back(pixel);
            }
            if (canRight) {
                pixel->neighbours.push_back(pixels[i - size.x + 1]);
                pixels[i - size.x + 1]->neighbours.push_back(pixel);
            }
            pixel->neighbours.push_back(pixels[i - size.x]);
            pixels[i - size.x]->neighbours.push_back(pixel);
        }
        if (canLeft) {
            pixel->neighbours.push_back(pixels[i - 1]);
            pixels[i - 1]->neighbours.push_back(pixel);
        }
        // if (canRight)
        //     pixel->neighbours.push_back(pixels[i + 1]);
        // if (canBottom) {
        //     if (canLeft)
        //         pixel->neighbours.push_back(pixels[i + size.x - 1]);
        //     if (canRight)
        //         pixel->neighbours.push_back(pixels[i + size.x + 1]);
        //     pixel->neighbours.push_back(pixels[i + size.x]);
        // }
    }

    auto it = pixels.begin();
    while (it != pixels.end()) {
        const auto pixel = *it;
        std::for_each(pixel->neighbours.begin(), pixel->neighbours.end(), [&pixel](Pixel* neighbour) {
            if (neighbour != nullptr && neighbour->alive)
                pixel->aliveNeighbourCount++;
        });
        ++it;
    }

    // std::cout << *this << std::endl;
}

void Texture::Update(sf::RenderWindow& window, sf::Vector2f& pixelSize) const {
    CalculateNextState();
    Render(window, pixelSize);
}

void Texture::CalculateNextState() const {
    PROFILE(*profiler, "calculate pixel");
    auto it = pixels.begin();
    while (it != pixels.end()) {
        PROFILE(*profiler, "pixel state logic");
        const auto pixel = *it;
        const bool isAlive = pixel->alive;
        const int liveNeighbors = pixel->aliveNeighbourCount;

        if (isAlive && (liveNeighbors < 2 || liveNeighbors > 3)) {
            // underpopulation or overpopulation
            pixel->alive = false;
        }else if (liveNeighbors == 3) {
            // reproduction
            pixel->alive = true;
        }
        ++it;
    }

    it = pixels.begin();
    while (it != pixels.end()) {
        PROFILE(*profiler, "neighbour counting logic");
        const auto pixel = *it;
        pixel->aliveNeighbourCount = 0;
        for (auto neighbour: pixel->neighbours) {
            pixel->aliveNeighbourCount += neighbour->alive;
        }
        ++it;
    }
}

void Texture::Render(sf::RenderWindow& window, sf::Vector2f& pixelSize) const {
    // int counter = 0;
    auto it = pixels.begin();
    while (it != pixels.end()) {
        const auto pixel = *it;
        PROFILE(*profiler, "pixel render");
        if (pixel->alive)
            window.draw(*pixel->render);
        ++it;
    }
}

std::ostream& operator<<(std::ostream& os, const Texture& texture) {
    os << texture.name << std::endl;
    os << texture.size.x << ", " << texture.size.y << std::endl;
    os << "pixels:" << std::endl;

    // std::stringstream afterPrint{};
    for (size_t i = 0; i < texture.pixels.size(); ++i) {
        auto pixel = texture.pixels[i];

        // if (i % texture.size.x == 0) {
        //     os << std::endl;
        // }
        // os << pixel->alive << " ";

        // afterPrint << std::endl << "Pixel:" << std::endl;
        os << "state = " << pixel->alive << " " << pixel << std::endl;
        os << "AliveNeighbourCount = " << pixel->aliveNeighbourCount << std::endl;
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
        std::for_each(pixel->neighbours.begin(), pixel->neighbours.end(), [&os](Pixel* neighbour) {
            if (neighbour != nullptr)
                os << "neighbour = " << neighbour->alive << " " << neighbour << std::endl;
        });
        os << "]" << std::endl;

    }
    // os << std::endl << afterPrint.str() << std::endl;

    return os;
}
