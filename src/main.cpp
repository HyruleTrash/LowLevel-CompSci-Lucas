#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

#include "Profiler.hpp"
#include "Texture.h"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 720, 720 }), "Conway's game of life");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;
    Profiler profiler;

    const auto windowSize = window.getSize();
    const sf::Vector2u texSize{30,30};

    sf::RectangleShape pixelTemplate;
    pixelTemplate.setSize({static_cast<float>(windowSize.x / texSize.x), static_cast<float>(windowSize.y / texSize.y)});
    auto pixelSize = pixelTemplate.getSize();

    const auto conwayTex = Texture(pixelTemplate, texSize, windowSize, profiler);
    // std::cout << conwayTex << std::endl;

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
        sf::Time deltaTime = deltaClock.restart();
        ImGui::SFML::Update(window, deltaTime);

        // Render
        window.clear();

        conwayTex.Update(window, pixelSize);
        #ifndef NDEBUG
        profiler.renderImGui();
        #endif

        ImGui::SFML::Render(window);
        window.display();
    }

	return 0;
}
