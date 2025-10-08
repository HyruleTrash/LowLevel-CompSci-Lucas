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

    auto it = buckets.find(hashPosition);
    if (it == buckets.end()) {
        buckets[hashPosition] = std::vector<std::shared_ptr<Ball>>{};
    }

    buckets[hashPosition].push_back(obj);

    // auto [tempX, tempY] = hashPosition;
    // std::cout << "instance: [" << std::endl << obj->shape.getPosition().x << ", " << obj->shape.getPosition().y << std::endl;
    // std::cout << std::get<0>(obj->GetPosition()) << ", " << std::get<1>(obj->GetPosition()) << std::endl;
    // std::cout << tempX << ", " << tempY << std::endl << "]" << std::endl;

    return hashPosition;
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
    const auto it = buckets.find(hashPosition);
    if (it == buckets.end()) {
        buckets.erase(hashPosition);
    }else if (it->second.empty()){
        buckets.erase(hashPosition);
    }
}

/// returns: a array of pointers to neighbouring buckets, buckets that don't exist will be nullptr
std::array<std::vector<std::shared_ptr<Ball>> *, 24> SpatialHash::GetNeighboringBuckets(std::tuple<int, int> hashPosition1) {
    std::array<std::vector<std::shared_ptr<Ball>> *, 24> result{};

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

std::ostream& operator<<(std::ostream& os, const SpatialHash& spatialHash) {
    const auto map = spatialHash.buckets;
    os << "spatialHash (" << spatialHash.buckets.size() << ") {\n";
    for (auto it = map.begin(); it != map.end(); ++it) {
        os << "{\n";
        const auto [keyX, keyY] = it->first;
        os << "  key: " << keyX << ", " << keyY << std::endl;
        const auto bucket = it->second;
        os << "  bucket: (" << bucket.size() << ") [";
        if (!bucket.empty()) {
            os << std::endl;
            for (auto it2 = bucket.begin(); it2 != bucket.end(); ++it2) {
                const auto obj = *it2;
                os << "    objPtr: " << it2->get() << " {" <<std::endl;

                const auto [otherKeyX, otherKeyY] = obj->hashPosition;
                os << "      savedKey: " << otherKeyX << ", " << otherKeyY << std::endl;

                const auto pos = obj->shape.getPosition();
                os << "      SFMLPos: " << pos.x << ", " << pos.y << std::endl;

                const auto [calculatedPosX, calculatedPosY] = obj->GetPosition();
                os << "      Pos: " << calculatedPosX << ", " << calculatedPosY << std::endl << "    }" << std::endl;
            }
        }
        os << "  ]\n";
        os << "}\n";
    }
    os << "}\n";
    return os;
}