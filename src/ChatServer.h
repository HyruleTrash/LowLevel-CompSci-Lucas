//
// Created by Lucas on 09/10/2025.
//

#ifndef LOW_LEVEL_SETUP_CHATSERVER_H
#define LOW_LEVEL_SETUP_CHATSERVER_H

#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <vector>

struct Client {
    std::string username;
    std::string identity;
    std::chrono::steady_clock::time_point last_ping;
    bool authenticated = false;
};

class ChatServer {
private:
    zmq::context_t context{};
    zmq::socket_t socket{};
    std::unordered_map<std::string, Client> clients; // identity -> client
    std::unordered_map<std::string, std::string> usernames; // username -> identity

public:
    ChatServer(int port);

    void Run();

private:
    void HandleMessage(zmq::multipart_t& msg);
    void HandleConnect(const std::string& identity);
    void HandlePong(const std::string& identity);
    void HandleSetUsername(const std::string& identity, const std::string& username);

    void HandlePrivateMessage(const std::string& sender_identity, const std::string& target, const std::string& message);
    void HandlePublicMessage(const std::string& sender_identity, const std::string& message);

    void HandleLeftMessage(const std::string& sender_identity);
    void SendDisconnectMessage(const std::string& sender_identity);

    void SendMessage(const std::string& identity, const std::string& msg_type);
    void SendMessage(const std::string& identity, const std::string& msg_type, const std::string& data);
    void SendMessage(const std::string& identity, const std::string& msg_type, const std::string& data1, const std::string& data2);

    void PingClients();

    void DisconnectClient(const std::string& identity);
};


#endif //LOW_LEVEL_SETUP_CHATSERVER_H