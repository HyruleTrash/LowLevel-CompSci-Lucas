//
// Created by Lucas on 18/09/2025.
//

#ifndef LOW_LEVEL_SETUP_TEXTURE_H
#define LOW_LEVEL_SETUP_TEXTURE_H
#include <list>
#include <string>
#include <vector>

#include "SFML/System/Vector2.hpp"


class Texture {
public:
    Texture();
    Texture(const Texture&);
    Texture(Texture&&) noexcept;
    ~Texture() = default;
    Texture& operator=(Texture);
    Texture& operator=(Texture&&) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);
public:
    sf::Vector2<int> size;
    std::vector<bool> pixels{};
    std::string name;
};


#endif //LOW_LEVEL_SETUP_TEXTURE_H