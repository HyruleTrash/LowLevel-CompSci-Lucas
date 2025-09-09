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

    // int number = 0x12345678;
    // unsigned char* bytes = (unsigned char*)&number;
    // for (int i = 0; i < 4; i++) {
    //     std::cout << std::hex << (int)bytes[i] << " ";
    // }
    // std::cout << std::endl;

    // struct BadEntity {
    //     bool active;
    //     double precise_x;
    //     bool visible;
    //     double precise_y;
    //     int health;
    //     bool can_move;
    // };
    //
    // std::cout << sizeof(BadEntity) << std::endl;
    //
    // struct GoodEntity {
    //     double precise_x; // 8
    //     double precise_y; // 8
    //     int health; // 4
    //     bool active; // 1
    //     bool visible; // 1
    //     bool can_move; // 1
    // };
    //
    // std::cout << sizeof(GoodEntity) << std::endl;

    // unsigned int temp = 3329;
    // std::cout << std::bitset<13>(temp) << std::endl;

    bool walking = false;
    bool running = false;

    std::cout << (walking ^ running) << std::endl;

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
