//
// Created by Lucas on 31/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_BASICWAITINGTHREAD_H
#define PARTICLE_SYSTEM_ASSIGNMENT_BASICWAITINGTHREAD_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

class BasicWaitingThread {
public:
    BasicWaitingThread(std::function<void()> func);
    ~BasicWaitingThread();

    void Start();
    void Reset();
    void Stop();

private:
    bool stop{false};
    bool shouldRun{false};
    std::mutex mutex{};
    std::condition_variable condition{};
    std::thread thread;
    std::function<void()> func;
};


#endif //PARTICLE_SYSTEM_ASSIGNMENT_BASICWAITINGTHREAD_H