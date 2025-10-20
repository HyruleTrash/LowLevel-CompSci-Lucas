//
// Created by Lucas on 09/10/2025.
//

#ifndef LOW_LEVEL_SETUP_CHATCLIENT_H
#define LOW_LEVEL_SETUP_CHATCLIENT_H

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

class ChatClient {
private:
    zmq::context_t context;
    zmq::socket_t socket;
    std::atomic<bool> connected{ false };
    std::atomic<bool> authenticated{ false };
    std::atomic<bool> running{ true };
    std::thread receive_thread;

    std::vector<std::string> messages;
    std::string username;
    std::string server_address = "tcp://localhost:5555";

    char username_buffer[64] = "";
    char target_buffer[64] = "";
    char message_buffer[256] = "";
    char server_buffer[128] = "tcp://localhost:5555";

public:
    ChatClient();
    ~ChatClient();

    void ConnectToServer();
    void Disconnect();

    void SetUsername();
    void SendPrivateMessage();
    void SendPublicMessage();
    void SendLeaveMessage();

    void Leave();

    void RenderGui();

private:
    void SendMessage(const std::string& msg_type);
    void SendMessage(const std::string& msg_type, const std::string& data);
    void SendMessage(const std::string& msg_type, const std::string& data1, const std::string& data2);

    void ReceiveLoop();

    void HandleServerMessage(zmq::multipart_t& msg);

    void AddMessage(const std::string& message);
};


#endif //LOW_LEVEL_SETUP_CHATCLIENT_H