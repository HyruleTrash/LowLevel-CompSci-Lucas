//
// Created by Lucas on 07/10/2025.
//

#include "Ball.h"

std::tuple<float, float> Ball::GetPosition() const {
    return {shape.getPosition().x,shape.getPosition().y};
}
