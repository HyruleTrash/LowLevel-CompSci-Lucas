//
// Created by Lucas on 18/09/2025.
//

#include "Texture.h"

#include <algorithm>
#include <cmath>
#include <ostream>

Texture::Texture()  : name("Texture"), size(30, 30){
    pixels.resize(size.x * size.y);
}

Texture::Texture(const Texture& source) : name(source.name), size(source.size) {
    pixels.resize(size.x * size.y);
    std::copy(source.pixels.begin(), source.pixels.end(), pixels.begin());
}

Texture::Texture(Texture&& source) noexcept
    : name(std::move(source.name))
    , size(source.size)
    , pixels(std::move(source.pixels)) {}

Texture& Texture::operator=(Texture other) {
    if (this == &other)
        return *this;

    name = other.name;
    size = other.size;

    pixels.resize(size.x * size.y);
    std::copy(other.pixels.begin(), other.pixels.end(), pixels.begin());

    return *this;
}

Texture & Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        size = other.size;
        pixels = std::move(other.pixels);
    }
    return *this;
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
