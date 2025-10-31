#ifndef MULTITHREADING_EXAMPLE_HPP
#define MULTITHREADING_EXAMPLE_HPP

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <functional>
#include <cmath>
#include <string>
#include "WorkerThreadPool.h"

struct JobResult {
    int id;
    float result;
    bool completed;
};

class MultithreadingDemo {
private:
    // Job parameters
    int numJobs = 100;
    int jobDurationMs = 100;

    // Processing state
    std::vector<JobResult> results;
    std::mutex resultsMutex;
    bool processing = false;
    std::atomic<int> completedJobs{ 0 };

    // Timing
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    double lastProcessingTime = 0.0;

    // Thread pool
    std::unique_ptr<WorkerThreadPool> threadPool;
    int numThreads = 4;
    bool useMultithreading = true;

    // UI state
    float progressBar = 0.0f;
    std::vector<std::pair<float, float>> performanceHistory; // time, completion percentage

    void processJobSingleThreaded(int jobId) {
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(jobDurationMs));

        // Compute result (simple calculation for demonstration)
        float result = std::sin(jobId * 0.1f) * 100.0f;

        JobResult jr;
        jr.id = jobId;
        jr.result = result;
        jr.completed = true;

        std::lock_guard<std::mutex> lock(resultsMutex);
        results[jobId] = jr;
        ++completedJobs;
    }

    void processJobMultithreaded(int jobId) {
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(jobDurationMs));

        // Compute result
        const float result = std::sin(jobId * 0.1f) * 100.0f;

        JobResult jr;
        jr.id = jobId;
        jr.result = result;
        jr.completed = true;

        std::lock_guard<std::mutex> lock(resultsMutex);
        results[jobId] = jr;
        ++completedJobs;
    }

    void startProcessing() {
        if (processing) return;

        processing = true;
        completedJobs = 0;
        results.clear();
        results.resize(numJobs);
        performanceHistory.clear();

        startTime = std::chrono::high_resolution_clock::now();

        if (useMultithreading) {
            threadPool = std::make_unique<WorkerThreadPool>(numThreads);

            std::thread([this]() {
                for (int i = 0; i < numJobs; ++i) {
                    threadPool->enqueue([this, i]() {
                        processJobMultithreaded(i);
                        });
                }

                // Wait for completion
                while (!threadPool->isIdle()) {
                    // std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

                endTime = std::chrono::high_resolution_clock::now();
                lastProcessingTime = std::chrono::duration<double>(endTime - startTime).count();
                processing = false;
                }).detach();
        }
        else {
            std::thread([this]() {
                for (int i = 0; i < numJobs; ++i) {
                    processJobSingleThreaded(i);
                }

                endTime = std::chrono::high_resolution_clock::now();
                lastProcessingTime = std::chrono::duration<double>(endTime - startTime).count();
                processing = false;
                }).detach();
        }
    }

    void updatePerformanceHistory() {
        if (processing) {
            const auto now = std::chrono::high_resolution_clock::now();
            float elapsed = std::chrono::duration<float>(now - startTime).count();
            float completion = static_cast<float>(completedJobs) / numJobs;
            performanceHistory.push_back({ elapsed, completion });

            // Keep only last 1000 points
            if (performanceHistory.size() > 1000) {
                performanceHistory.erase(performanceHistory.begin());
            }
        }
    }

public:
    MultithreadingDemo() = default;
    ~MultithreadingDemo() = default;

    // Call this in your main loop update function
    void update() {
        updatePerformanceHistory();
    }

    // Call this in your ImGui rendering section
    void renderImGui() {
        ImGui::Begin("Multithreading Demo");

        ImGui::Text("Job Configuration");
        ImGui::Separator();

        ImGui::SliderInt("Number of Jobs", &numJobs, 10, 500);
        ImGui::SliderInt("Job Duration (ms)", &jobDurationMs, 1, 500);

        ImGui::Spacing();
        ImGui::Text("Thread Configuration");
        ImGui::Separator();

        ImGui::Checkbox("Use Multithreading", &useMultithreading);
        if (useMultithreading) {
            ImGui::SliderInt("Number of Threads", &numThreads, 1, 16);
        }

        ImGui::Spacing();

        if (processing) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::Button("Processing...", ImVec2(200, 40));
            ImGui::PopStyleColor();
        }
        else {
            if (ImGui::Button("Start Processing", ImVec2(200, 40))) {
                startProcessing();
            }
        }

        ImGui::Spacing();
        ImGui::Text("Status");
        ImGui::Separator();

        progressBar = static_cast<float>(completedJobs) / numJobs;
        ImGui::ProgressBar(progressBar, ImVec2(-1, 0),
            (std::to_string(completedJobs) + " / " + std::to_string(numJobs)).c_str());

        if (lastProcessingTime > 0.0) {
            ImGui::Text("Last Processing Time: %.3f seconds", lastProcessingTime);
            ImGui::Text("Jobs per Second: %.1f", numJobs / lastProcessingTime);
        }

        // Performance graph
        if (!performanceHistory.empty()) {
            ImGui::Spacing();
            ImGui::Text("Completion Progress Over Time");

            std::vector<float> plotData;
            for (const auto& p : performanceHistory) {
                plotData.push_back(p.second);
            }

            ImGui::PlotLines("##progress", plotData.data(), plotData.size(),
                0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        }

        // Results preview
        if (!results.empty()) {
            ImGui::Spacing();
            ImGui::Text("Results Preview (First 10 Jobs)");
            ImGui::Separator();

            ImGui::BeginChild("Results", ImVec2(0, 150), true);
            int count = 0;
            for (const auto& result : results) {
                if (result.completed) {
                    ImGui::Text("Job %d: Result = %.2f", result.id, result.result);
                    count++;
                    if (count >= 10) break;
                }
            }
            ImGui::EndChild();
        }

        ImGui::End();
    }

    // Getters for status
    bool isProcessing() const { return processing; }
    int getCompletedJobs() const { return completedJobs; }
    int getTotalJobs() const { return numJobs; }
    double getLastProcessingTime() const { return lastProcessingTime; }
};

#endif // MULTITHREADING_EXAMPLE_HPP