//
// Created by Lucas on 27/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_GAMEOBJECT_H
#define PARTICLE_SYSTEM_ASSIGNMENT_GAMEOBJECT_H
#include <SFML/Graphics.hpp>


class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual bool isAlive() const = 0;

protected:
    bool active;
    int id;
    float x, y;
    double creationTime;
};


#endif //PARTICLE_SYSTEM_ASSIGNMENT_GAMEOBJECT_H