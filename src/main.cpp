#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

#include "Texture.h"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 1280, 720 }), "My test window");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;

    auto conwayTex = Texture();
    auto windowSize = window.getSize();
    sf::RectangleShape pixelTemplate;
    pixelTemplate.setScale({static_cast<float>(windowSize.x / conwayTex.size.x), static_cast<float>(windowSize.y / conwayTex.size.y)});

    while (window.isOpen())
    {
        // Event Polling
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        conwayTex.Render(window, pixelTemplate);

        // Update
        ImGui::SFML::Update(window, deltaClock.restart());
        // ImGui::ShowDemoWindow();

        // Render
        window.clear();

        ImGui::SFML::Render(window);

        window.display();
    }

	return 0;
}
