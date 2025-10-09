#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <SFML/Graphics.hpp>

#include <queue>
#include <thread>
#include <iostream>

int counter = 0;
bool done = false;

std::queue<int> goods;

void producer() {
    std::cout << "Starting producer..." << std::endl;

    for (int i = 0; i < 500; ++i) {
        goods.push(i);
        counter++;
    }

    done = true;

    std::cout << "Finished producer..." << std::endl;
}

void consumer() {
    std::cout << "Starting consumer..." << std::endl;

    while (!done) {
        while (!goods.empty()) {
            goods.pop();
            counter--;
        }
    }

    std::cout << "Finished consumer..." << std::endl;
}

int main() {
    /*
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
    */
    counter = 0;
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    std::cout << "Net: " << counter << " " << goods.size() << std::endl;
    return 0;
}
