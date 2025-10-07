//
// Created by Lucas on 06/10/2025.
//

#include "SpatialHash.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

SpatialHash::SpatialHash(int* cellSize) : cellSize(cellSize), buckets(0, HashPositionToKeyFunction()) {}

std::tuple<int, int> SpatialHash::HashFunction(std::tuple<float, float> position) const {
    auto [x, y] = position;
    int cellX = static_cast<int>(std::floor(x / *cellSize));
    int cellY = static_cast<int>(std::floor(y / *cellSize));
    return {cellX, cellY};
}

/// return: current position in hashmap
std::tuple<int, int> SpatialHash::Insert(const std::shared_ptr<Ball>& obj) {
    const auto hashPosition = HashFunction(obj->GetPosition());
    auto [iterator, success] = buckets.emplace(
        hashPosition,
        std::vector<std::shared_ptr<Ball>>{}
    );
    if (success)
        buckets[hashPosition].push_back(obj);

    // auto [tempX, tempY] = hashPosition;
    // std::cout << "instance: [" << std::endl << obj->shape.getPosition().x << ", " << obj->shape.getPosition().y << std::endl;
    // std::cout << std::get<0>(obj->GetPosition()) << ", " << std::get<1>(obj->GetPosition()) << std::endl;
    // std::cout << tempX << ", " << tempY << std::endl << "]" << std::endl;

    return hashPosition;
}

/// return: current position in hashmap
std::tuple<int, int> SpatialHash::Update(const std::shared_ptr<Ball>& obj) {
    const auto oldHashPosition = obj->hashPosition;
    auto& oldBucket = buckets[oldHashPosition];

    if (const auto it = std::find(oldBucket.begin(), oldBucket.end(), obj); it != oldBucket.end()) {
        oldBucket.erase(it);
        CheckBucketEmpty(oldHashPosition);
    }

    return Insert(obj);
}

/// return: current position in hashmap
void SpatialHash::Remove(const std::shared_ptr<Ball>& obj) {
    const auto hashPosition = obj->hashPosition;
    if (buckets.find(hashPosition) != buckets.end()) {
        auto& bucket = buckets[hashPosition];
        if (const auto it = std::find(bucket.begin(), bucket.end(), obj); it != bucket.end()) {
            bucket.erase(it);
        }
    }
    CheckBucketEmpty(hashPosition);
}

void SpatialHash::CheckBucketEmpty(std::tuple<int, int> hashPosition) {
    if (buckets.find(hashPosition) == buckets.end()) {
        buckets.erase(hashPosition);
    }
}

/// returns: a array of pointers to neighbouring buckets, buckets that don't exist will be nullptr
std::array<std::vector<std::shared_ptr<Ball>> *, 8> SpatialHash::GetNeighboringBuckets(std::tuple<int, int> hashPosition1) {
    std::array<std::vector<std::shared_ptr<Ball>> *, 8> result{};

    const auto [hashPosition1X, hashPosition1Y] = hashPosition1;
    for (size_t i = 0; i < ADJACENT_KEYS.size(); ++i) {
        const auto [otherKeyX, otherKeyY] = ADJACENT_KEYS[i];
        const std::tuple<int, int> hashPosition2 = {hashPosition1X + otherKeyX, hashPosition1Y + otherKeyY};
        if (buckets.find(hashPosition2) != buckets.end()) {
            result[i] = &buckets[hashPosition2];
        }
    }
    return result;
}