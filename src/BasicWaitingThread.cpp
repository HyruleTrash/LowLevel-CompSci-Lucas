//
// Created by Lucas on 31/10/2025.
//

#include "BasicWaitingThread.h"

BasicWaitingThread::BasicWaitingThread(std::function<void()> func) : func(std::move(func)), stop(false), shouldRun(false) {
    thread = std::thread([this]() {
        while (!stop) {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [this]{ return shouldRun || stop; });
            if (stop) break;
            if (shouldRun) {
                shouldRun = false;
                this->func();
            }
        }
    });
}

BasicWaitingThread::~BasicWaitingThread() {
    Stop();
}

void BasicWaitingThread::Start() {
    std::lock_guard<std::mutex> lock(mutex);
    shouldRun = true;
    condition.notify_one();
}

void BasicWaitingThread::Stop() {
    std::lock_guard<std::mutex> lock(mutex);
    stop = true;
    condition.notify_all();
    if (thread.joinable()) {
        thread.join();
    }
}
