//
// Created by Lucas on 15/09/2025.
//

#ifndef LOW_LEVEL_SETUP_OFFSCREENTELEPORTER_H
#define LOW_LEVEL_SETUP_OFFSCREENTELEPORTER_H
#include "Component.h"


class OffScreenTeleporter : public Component{
public:
    void Update(sf::RenderWindow&, Profiler&, sf::Time& deltaTime) override;
};

#endif //LOW_LEVEL_SETUP_OFFSCREENTELEPORTER_H
