//
// Created by Lucas on 18/09/2025.
//

#include "Texture.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <ostream>

Texture::Texture(const sf::RectangleShape& pixelTemplate, const sf::Vector2u& texSize, const sf::Vector2u& windowSize, Profiler& profiler)  : name("Texture"), size(texSize.x, texSize.y){
    this->profiler = &profiler;

    pixels.resize(size.x * size.y);
    auto pixelSize = pixelTemplate.getSize();

    for (size_t i = 0; i < pixels.size(); ++i) {
        const auto pixel = new Pixel();
        pixel->render = std::make_unique<sf::RectangleShape>(pixelTemplate);
        const auto pos = pixel->render->getPosition();

        const int modX = i % size.x;
        const int row = i / size.x;

        const float x = static_cast<float>(modX) * pixelSize.x;
        const float y = static_cast<float>(row) * pixelSize.y;
        pixel->render->setPosition({x, y});

        const bool canLeft = modX != 0;
        const bool canRight = modX != size.x - 1;
        const bool canTop = row != 0;
        const bool canBottom = row != size.y - 1;
        if (canTop) {
            if (canLeft)
                pixel->neighbour100 = pixels[i - size.x - 1];
            if (canRight)
                pixel->neighbour010 = pixels[i - size.x + 1];
            pixel->neighbour001 = pixels[i - size.x];
        }
        if (canLeft)
            pixel->neighbour000 = pixels[i - 1];
        if (canRight)
            pixel->neighbour111 = pixels[i + 1];
        if (canBottom) {
            if (canLeft)
                pixel->neighbour011 = pixels[i + size.x - 1];
            if (canRight)
                pixel->neighbour101 = pixels[i + size.x + 1];
            pixel->neighbour110 = pixels[i + size.x];
        }

        #ifndef NDEBUG
        const std::uint8_t g = x / static_cast<float>(windowSize.x) * 255.0;
        const std::uint8_t b = y / static_cast<float>(windowSize.y) * 255.0;
        const auto color = sf::Color(0, g, b, 255);
        pixel->render->setFillColor(color);
        #endif

        pixels[i] = pixel;
    }
}

void Texture::Update(sf::RenderWindow& window, sf::Vector2f& pixelSize) const {
    CalculateNextState();
    Render(window, pixelSize);
}

void Texture::CalculateNextState() const {
}

void Texture::Render(sf::RenderWindow& window, sf::Vector2f& pixelSize) const {
    // int counter = 0;
    auto it = pixels.begin();
    while (it != pixels.end()) {
        const auto pixel = *it;
        PROFILE(*profiler, "pixel render");
        // if (pixel->state)
        //     pixel->render->setFillColor(sf::Color::White);
        // else
        //     pixel->render->setFillColor(sf::Color::Black);

        window.draw(*pixel->render);
        ++it;
    }
}

std::ostream& operator<<(std::ostream& os, const Texture& texture) {
    os << texture.name << std::endl;
    os << texture.size.x << ", " << texture.size.y << std::endl;
    os << "pixels:" << std::endl;

    std::stringstream afterPrint{};
    for (size_t i = 0; i < texture.pixels.size(); ++i) {
        auto pixel = texture.pixels[i];

        if (i % texture.size.x == 0) {
            os << std::endl;
        }
        os << pixel->state << " ";

        afterPrint << std::endl << "Pixel:" << std::endl;
        afterPrint << "state = " << pixel->state << std::endl;
        afterPrint << "render = " << pixel->render.get() << std::endl;
        if (pixel->render != nullptr) {
            afterPrint << "render [ " << std::endl;
            auto color = pixel->render->getFillColor();
            afterPrint << "color = " << color.r << ", " << color.g << ", " << color.b << std::endl;
            auto pos = pixel->render->getPosition();
            afterPrint << "position = " << pos.x << ", " << pos.y << std::endl;
            auto size = pixel->render->getSize();
            afterPrint << "size = " << size.x << ", " << size.y << std::endl;
            afterPrint << "]" << std::endl;
        }
    }
    os << std::endl << afterPrint.str() << std::endl;

    return os;
}
