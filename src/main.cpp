#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

#include "Profiler.hpp"
#include "BallSimulation.h"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 1280, 720 }), "Collision crisis", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;
    const auto profiler = std::make_shared<Profiler>();
    const auto ballSimulation = std::make_unique<BallSimulation>(profiler);
    const auto windowSize = sf::Vector2u(window.getSize());

    while (window.isOpen())
    {
        PROFILE(*profiler, "Frame call");
        // Event Polling
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>()) {
                profiler->clear();
                window.close();
            }
        }

        // Update
        auto dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);
        ballSimulation->UpdateBalls(windowSize, dt.asSeconds());

        // Render
        window.clear();

        ballSimulation->drawBalls(window);

        // #ifndef NDEBUG
        profiler->renderImGui();
        // #endif

        ImGui::SFML::Render(window);

        window.display();
    }

	return 0;
}
