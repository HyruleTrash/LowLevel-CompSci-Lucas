#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

#include "Profiler.hpp"
#include "Texture.h"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 900, 900 }), "Conway's game of life");
    window.setFramerateLimit(60); // change later
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;
    Profiler profiler;

    const auto windowSize = window.getSize();
    const sf::Vector2u texSize{300,300};

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
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
                    // conwayTex.CalculateNextState();
                    std::cout << conwayTex << std::endl;
                }
            }
        }

        // Update
        sf::Time deltaTime = deltaClock.restart();
        ImGui::SFML::Update(window, deltaTime);

        // Render
        window.clear();

        // conwayTex.Render(window, pixelSize);
        conwayTex.Update(window, pixelSize);

        #ifndef NDEBUG
        profiler.renderImGui();
        #endif

        ImGui::SFML::Render(window);
        window.display();
    }

	return 0;
}
