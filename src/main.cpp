#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

#include "ParticleSystem.h"
#include "Profiler.hpp"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 720, 720 }), "Particle System");
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;
    const auto profiler = std::make_shared<Profiler>();
    const auto particleSystem = new ParticleSystem(&window, profiler);

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
                const auto button = event->getIf<sf::Event::MouseButtonPressed>();
                if (button->button == sf::Mouse::Button::Right)
                    particleSystem->TriggerSpawnParticles(60000, sf::Vector2f(button->position));
            }
            // else if (event->is<sf::Event::KeyPressed>()) {
            //     const auto button = event->getIf<sf::Event::KeyPressed>();
            //     if (button->code == sf::Keyboard::Key::Enter)
            //         particleSystem->breaker = true;
            // }
        }

        auto dt = deltaClock.restart();

        // Update
        ImGui::SFML::Update(window, dt);
        // ImGui::ShowDemoWindow();

        particleSystem->Update(dt.asSeconds());

        // Render
        window.clear();

        particleSystem->Render();
        profiler->renderImGui();
        ImGui::SFML::Render(window);

        window.display();
    }

	return 0;
}