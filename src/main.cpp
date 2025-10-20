#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({ 1280, 720 }), "My test window");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!ImGui::SFML::Init(window))
        return -1;

    sf::Clock deltaClock;

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

        // Update
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::ShowDemoWindow();

        // Render
        window.clear();

        ImGui::SFML::Render(window);

        window.display();
    }

	return 0;
}

// int main(int argc, char* argv[]) {
//     int port = 5555;
//
//     if (argc > 1) {
//         port = std::atoi(argv[1]);
//     }
//
//     try {
//         ChatServer server(port);
//         server.run();
//     }
//     catch (const std::exception& e) {
//         std::cerr << "Server error: " << e.what() << std::endl;
//         return 1;
//     }
//
//     return 0;
// }