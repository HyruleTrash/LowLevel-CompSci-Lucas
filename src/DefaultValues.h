//
// Created by Lucas on 03/10/2025.
//

#ifndef LOW_LEVEL_SETUP_DEFAULTVALUES_H
#define LOW_LEVEL_SETUP_DEFAULTVALUES_H

class TextureConstants {
private:
    // Private constructor to prevent instantiation
    TextureConstants() = delete;
    // Delete copy operations
    TextureConstants(const TextureConstants&) = delete;
    TextureConstants& operator=(const TextureConstants&) = delete;

public:
    // Static accessor for the constants
    static constexpr int getXSize() { return 300; }
    static constexpr int getYSize() { return 300; }
    static constexpr int getSize() { return getXSize() * getYSize(); }
};

#endif //LOW_LEVEL_SETUP_DEFAULTVALUES_H