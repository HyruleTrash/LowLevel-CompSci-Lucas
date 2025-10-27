#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

#include "ParticleSystem.h"
#include "Profiler.hpp"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 720, 720 }), "My test window");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;
    const auto profiler = std::make_shared<Profiler>();
    const auto particleSystem = new ParticleSystem(&window);

    while (window.isOpen())
    {
        PROFILE(*profiler, "Frame call");
        // Event Polling
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (event->is<sf::Event::MouseButtonPressed>())
            {
                particleSystem->spawnParticles(60000, sf::Vector2f(event->getIf<sf::Event::MouseButtonPressed>()->position));
            }
        }

        auto dt = deltaClock.restart();

        // Update
        ImGui::SFML::Update(window, dt);
        // ImGui::ShowDemoWindow();

        particleSystem->update(dt.asSeconds());

        // Render
        window.clear();

        particleSystem->render();
        profiler->renderImGui();
        ImGui::SFML::Render(window);

        window.display();
    }

	return 0;
}
