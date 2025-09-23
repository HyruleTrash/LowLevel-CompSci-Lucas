//
// Created by Lucas on 18/09/2025.
//

#include "Texture.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <ostream>

Texture::Texture(const sf::RectangleShape& pixelTemplate, sf::Vector2u texSize, sf::Vector2u windowSize)  : name("Texture"), size(texSize.x, texSize.y){
    pixels.resize(size.x * size.y);
    auto pixelSize = pixelTemplate.getSize();
    int counter = 0;

    for (size_t i = 0; i < pixels.size(); ++i) {
        const auto pixel = new Pixel();
        pixel->render = std::make_unique<sf::RectangleShape>(pixelTemplate);
        const auto pos = pixel->render->getPosition();

        pixel->render->setFillColor(sf::Color(0, pos.x / windowSize.x, pos.y / windowSize.y));

        int modX = i % size.x;
        pixel->render->setPosition({modX * pixelSize.x, counter * pixelSize.y});

        if (modX == 0) {
            counter++;
        }
        pixels[i] = pixel;
    }
}

void Texture::Render(sf::RenderWindow& window, sf::Vector2f& pixelSize) const {
    // int counter = 0;
    for (size_t i = 0; i < pixels.size(); ++i) {
        const auto pixel = pixels[i];
        // if (pixel->state)
        //     pixel->render->setFillColor(sf::Color::White);
        // else
        //     pixel->render->setFillColor(sf::Color::Black);

        // int modX = i % size.x;
        //
        // if (modX == 0) {
        //     counter++;
        // }

        window.draw(*pixel->render);
    }
}

std::ostream& operator<<(std::ostream& os, const Texture& texture) {
    os << texture.name << std::endl;
    os << texture.size.x << ", " << texture.size.y << std::endl;
    os << "pixels:" << std::endl;

    for (size_t i = 0; i < texture.pixels.size(); ++i) {
        bool pixel = texture.pixels[i];

        if (i % texture.size.x == 0) {
            os << std::endl;
        }
        os << pixel << " ";
    }
    os << std::endl;

    return os;
}
