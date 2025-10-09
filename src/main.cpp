#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <bitset>
#include <condition_variable>
#include <mutex>
#include <SFML/Graphics.hpp>

#include <queue>
#include <thread>
#include <iostream>

std::mutex mtx;
std::condition_variable cv;
int counter = 0;
bool done = false;

std::queue<int> goods;

void producer() {
    std::cout << "Starting producer..." << std::endl;
    std::lock_guard<std::mutex> lock(mtx);

    for (int i = 0; i < 500; ++i) {
        goods.push(i);
        counter++;
    }

    done = true;
    cv.notify_all();

    std::cout << "Finished producer..." << std::endl;
}

void consumer() {
    std::cout << "Starting consumer..." << std::endl;
    std::unique_lock lock(mtx);

    cv.wait(lock, []{ return done; });

    while (!goods.empty()) {
        goods.pop();
        counter--;
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
