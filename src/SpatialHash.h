//
// Created by Lucas on 06/10/2025.
//

#ifndef COLLISION_CRISIS_SPATIALHASH_H
#define COLLISION_CRISIS_SPATIALHASH_H
#include <iosfwd>
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
    void Remove(const std::shared_ptr<Ball>& obj);
    std::array<std::vector<std::shared_ptr<Ball>>*, 24> GetNeighboringBuckets(std::tuple<int, int> hashPosition1);
    friend std::ostream& operator<<(std::ostream& os, const SpatialHash& texture);
private:
    void CheckBucketEmpty(std::tuple<int, int> hashPosition);

public:
    static constexpr std::array<std::tuple<int, int>, 24> ADJACENT_KEYS = {{
        {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2},
        {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1},
        {-2, 0},  {-1, 0},              {1, 0},  {2, 0},
        {-2, 1},  {-1, 1},  {0, 1},  {1, 1},  {2, 1},
        {-2, 2},  {-1, 2},  {0, 2},  {1, 2},  {2, 2}
    }};
    int* cellSize;
    std::unordered_map<std::tuple<int, int>, std::vector<std::shared_ptr<Ball>>, HashPositionToKeyFunction> buckets;
};

#endif //COLLISION_CRISIS_SPATIALHASH_H
