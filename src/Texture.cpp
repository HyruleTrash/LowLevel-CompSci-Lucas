//
// Created by Lucas on 18/09/2025.
//

#include "Texture.h"

#include <algorithm>
#include <ostream>

Texture::Texture() {
    name = "Texture";
    size = 30 * 30;
    pixels = new int[size]();
}

Texture::Texture(const Texture& source) {
    name = source.name;
    size = source.size;
    pixels = new int[size]();
    std::copy_n(source.pixels, size, pixels);
}

Texture::Texture(Texture&& source) noexcept
    : name(std::move(source.name))
    , size(source.size)
    , pixels(source.pixels) {
    source.pixels = nullptr;
}

Texture::~Texture() {
    delete[] pixels;
}

Texture& Texture::operator=(Texture other) {
    if (this == &other)
        return *this;

    name = other.name;
    size = other.size;

    delete[] pixels;

    pixels = new int[size]();
    std::copy_n(other.pixels, size, pixels);

    return *this;
}

Texture & Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        delete pixels;
        name = std::move(other.name);
        size = other.size;
        pixels = other.pixels;
        other.pixels = nullptr;
    }
    return *this;
}

std::ostream & operator<<(std::ostream &os, const Texture &texture) {
    os << texture.name << std::endl;
    os << texture.size << std::endl;

    if (texture.pixels == NULL) {
        os << "no pixels" << std::endl;
    }else {
        os << "pixels:" << std::endl;
        for(int i = 0; i < texture.size; i++) {
            os << texture.pixels[i] << std::endl;
        }
    }
    return os << std::endl;
}
