
#ifndef LOW_LEVEL_SETUP_PROFILER_H
#define LOW_LEVEL_SETUP_PROFILER_H

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <random>

// Macro for easy profiling
// #ifdef NDEBUG
//     #define PROFILE(profiler, name) ((void)0)
// #else
    #define PROFILE(profiler, name) Profiler::ScopedTimer timer(profiler, name)
// #endif

class Profiler {
private:
    struct ProfileData {
        std::string name;
        float duration_ms;
        std::vector<float> history;
        static constexpr size_t MAX_HISTORY = 120; // 2 seconds at 60fps
    };

    std::vector<ProfileData> profiles;
    long frameCount{0};

public:
    class ScopedTimer {
        Profiler& profiler;
        std::string name;
        std::chrono::high_resolution_clock::time_point start_time;

    public:
        ScopedTimer(Profiler& p, const std::string& n)
            : profiler(p), name(n), start_time(std::chrono::high_resolution_clock::now()) {
        }

        ~ScopedTimer() {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            profiler.addSample(name, duration.count() / 1000.0f); // Convert to milliseconds
        }
    };

    void addSample(const std::string& name, float duration_ms) {
        // Find or create profile data
        auto it = std::find_if(profiles.begin(), profiles.end(),
            [&name](const ProfileData& data) { return data.name == name; });

        if (it == profiles.end()) {
            profiles.emplace_back();
            it = profiles.end() - 1;
            it->name = name;
        }

        it->duration_ms = duration_ms;
        it->history.push_back(duration_ms);

        // Keep history bounded
        if (it->history.size() > ProfileData::MAX_HISTORY) {
            it->history.erase(it->history.begin());
        }
    }

    void renderImGui() {
        ImGui::Begin("Performance Profiler");
        if (!std::filesystem::exists("imgui.ini")) {
            ImGui::SetWindowCollapsed(true);
        }
        ImGui::SetWindowPos({0,0});

        float total_frame_time = 0.0f;
        for (const auto& profile : profiles) {
            total_frame_time += profile.duration_ms;
        }

        ImGui::Text("Total Frame Time: %.3f ms (%.1f FPS) - frame count: %d",
            total_frame_time, 1000.0f / std::max(total_frame_time, 0.001f), frameCount);

        ImGui::Separator();

        for (const auto& profile : profiles) {
            if (profile.history.empty()) continue;

            // Calculate statistics
            float min_time = *std::min_element(profile.history.begin(), profile.history.end());
            float max_time = *std::max_element(profile.history.begin(), profile.history.end());
            float avg_time = 0.0f;
            for (float t : profile.history) avg_time += t;
            avg_time /= profile.history.size();

            // Display current time and statistics
            ImGui::Text("%s: %.3f ms", profile.name.c_str(), profile.duration_ms);
            ImGui::SameLine();
            ImGui::TextDisabled("(avg: %.3f, min: %.3f, max: %.3f)", avg_time, min_time, max_time);

            // Create the graph
            std::string graph_id = "##" + profile.name;
            ImGui::PlotLines(graph_id.c_str(),
                profile.history.data(),
                profile.history.size(),
                0,
                nullptr,
                0.0f,
                max_time * 1.2f, // Scale graph to show spikes
                ImVec2(0, 80));
        }

        ImGui::End();
    }

    void clear() {
        for (auto& profile : profiles) {
            profile.history.clear();
        }
    }

    void AddToFrameCounter() {
        ++frameCount;
    }
};

#endif //LOW_LEVEL_SETUP_PROFILER_H