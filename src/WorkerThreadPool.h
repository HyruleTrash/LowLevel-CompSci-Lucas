//
// Created by Lucas on 31/10/2025.
//

#ifndef PARTICLE_SYSTEM_ASSIGNMENT_WORKERTHREADPOOL_H
#define PARTICLE_SYSTEM_ASSIGNMENT_WORKERTHREADPOOL_H
#include <atomic>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

class WorkerThreadPool {
public:
    WorkerThreadPool(size_t numThreads);
    ~WorkerThreadPool();

    template<class F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }

    bool isIdle() const;
    int getQueueSize() const;
private:
    void threadFunc();

private:
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop{ false };
    std::atomic<int> activeTasks{ 0 };
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
};

#endif //PARTICLE_SYSTEM_ASSIGNMENT_WORKERTHREADPOOL_H