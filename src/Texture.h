//
// Created by Lucas on 18/09/2025.
//

#ifndef LOW_LEVEL_SETUP_TEXTURE_H
#define LOW_LEVEL_SETUP_TEXTURE_H
#include <string>


class Texture {
public:
    Texture();
    Texture(const Texture&);
    ~Texture();
    Texture& operator=(Texture);
    friend std::ostream& operator<<(std::ostream& os, const Texture& texture);
public:
    std::string name;
    int size;
    int* pixels;
};


#endif //LOW_LEVEL_SETUP_TEXTURE_H