#include <iostream>
#include <vector>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

#include "Ball.h"
#include "EntityManager.h"
#include "Profiler.hpp"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode::getDesktopMode(), "My test window", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;
    Profiler profiler;

    auto entityManager = EntityManager(window, profiler);
    entityManager.AddEntity(new Ball());

    while (window.isOpen())
    {
        PROFILE(profiler, "Frame Update");
        // Event Polling
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>()) {
                profiler.clear();
                window.close();
            }
        }

        // Update
        ImGui::SFML::Update(window, deltaClock.restart());
        // ImGui::ShowDemoWindow();

        // Render
        window.clear();
        entityManager.Update();

        #ifndef NDEBUG
        profiler.renderImGui();
        #endif

        ImGui::SFML::Render(window);

        window.display();
    }

	return 0;
}
