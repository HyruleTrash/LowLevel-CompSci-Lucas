//
// Created by Lucas on 23/09/2025.
//

#ifndef LOW_LEVEL_SETUP_PIXEL_NEIGHBOURS_H
#define LOW_LEVEL_SETUP_PIXEL_NEIGHBOURS_H
#include <cstdint>

struct PixelNeighbours {
    int aliveNeighbourCount{0};
    std::vector<size_t> neighbours;
};


#endif //LOW_LEVEL_SETUP_PIXEL_NEIGHBOURS_H