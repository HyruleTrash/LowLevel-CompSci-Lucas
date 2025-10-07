//
// Created by Lucas on 06/10/2025.
//

#include "SpatialHash.h"

#include <algorithm>
#include <memory>
#include <stdexcept>

SpatialHash::SpatialHash(int* cellSize) : cellSize(cellSize), buckets(0, HashPositionToKeyFunction()) {}

std::tuple<int, int> SpatialHash::HashFunction(std::tuple<float, float> position) const {
    auto [x, y] = position;
    return {x / *cellSize, y / *cellSize};
}

/// return: current position in hashmap
std::tuple<int, int> SpatialHash::Insert(const std::shared_ptr<Ball>& obj) {
    // const auto hasher = buckets.hash_function();
    const auto hashPosition = HashFunction(obj->GetPosition());
    // const auto key = hasher(hashPosition);
    if (buckets.find(hashPosition) == buckets.end()) {
        buckets[hashPosition] = std::vector<std::shared_ptr<Ball>>();
    }
    buckets[hashPosition].push_back(obj);
    return hashPosition;
}

/// return: current position in hashmap
std::tuple<int, int> SpatialHash::Update(const std::shared_ptr<Ball>& obj) {
    const auto oldHashPosition = obj->hashPosition;
    if (buckets.find(oldHashPosition) == buckets.end())
        return oldHashPosition;

    auto& oldBucket = buckets[oldHashPosition];
    const auto it = std::find(oldBucket.begin(), oldBucket.end(), obj);
    if (it == oldBucket.end())
        return oldHashPosition;

    const auto newHashPosition = HashFunction(obj->GetPosition());
    buckets[newHashPosition].push_back(std::move(*it));

    oldBucket.erase(it);
    CheckBucketEmpty(oldHashPosition);

    return newHashPosition;
}

/// return: current position in hashmap
void SpatialHash::Remove(const std::shared_ptr<Ball>& obj) {
    // const auto hasher = buckets.hash_function();
    const auto hashPosition = HashFunction(obj->GetPosition());
    // const auto key = hasher(hashPosition);
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
