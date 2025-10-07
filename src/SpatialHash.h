//
// Created by Lucas on 06/10/2025.
//

#ifndef COLLISION_CRISIS_SPATIALHASH_H
#define COLLISION_CRISIS_SPATIALHASH_H
#include <tuple>
#include <unordered_map>
#include <bits/shared_ptr_base.h>

#include "Ball.h"

class SpatialHash {
public:
    struct HashPositionToKeyFunction {
        std::size_t operator()(const std::tuple<int, int>& position) const {
            auto [x, y] = position;
            return std::hash<int>{}(x) ^ (std::hash<int>{}(y) << 1);
        }
    };
    SpatialHash(int* cellSize);
    SpatialHash(const SpatialHash&) = delete;
    ~SpatialHash() = default;
    std::tuple<int, int> HashFunction(std::tuple<float, float> position) const;
    std::tuple<int, int> Insert(const std::shared_ptr<Ball>& obj);
    std::tuple<int, int> Update(const std::shared_ptr<Ball>& obj);
    void Remove(const std::shared_ptr<Ball>& obj);

    void CheckBucketEmpty(std::tuple<int, int> hashPosition);

private:
    int* cellSize;
    std::unordered_map<std::tuple<int, int>, std::vector<std::shared_ptr<Ball>>, HashPositionToKeyFunction> buckets;
};

#endif //COLLISION_CRISIS_SPATIALHASH_H
