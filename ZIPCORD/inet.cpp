#include "inet.h"
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include "json.hpp"
#include "XOR_.h"
#include <thread>
#include "main.h"
#include <string>
#include <algorithm>
using json = nlohmann::json;
// Implementations (no class definition here)

SOCKET inet::cl = INVALID_SOCKET;

void inet::initsocket() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        std::cerr << "WSAStartup failed." << std::endl;

    cl = socket(AF_INET, SOCK_STREAM, 0);
    if (cl == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_port = htons(412);

    if (connect(cl, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) std::cerr << "Connect failed." << std::endl;
    
}

void inet::recv_() {

}

void inet::reconnect() {
    // Implementation
}

void inet::Send(json data) {
    try {
        std::string jsonStr = data.dump();
        send(cl, xory.XOR(jsonStr, 'e').c_str(), xory.XOR(jsonStr, 'e').size(), 0) != SOCKET_ERROR;
    }
    catch (...) {
        // Error handling
    }
}

json inet::Sendjr(json request) {
    Send(request);
    std::vector<char> buffer(1024);
    int bytes_received = recv(cl, buffer.data(), buffer.size(), 0);
    if (bytes_received <= 0)
        return json{ {"status", "error"}, {"message", "No response from server"} };

    std::string b(buffer.data(), bytes_received);
    std::string xor_ = xory.XOR(b, 'd');
    if (!xor_.empty() && xor_[0] == '{') {
        return json::parse(xor_);
    }
    return json{}; // Return empty json if parsing fails
}

SOCKET inet::getcls() {
    return cl;
}

void inet::stop() {
    if (cl != INVALID_SOCKET) {
        closesocket(cl);
    }
    WSACleanup();
}

int inet::getnewid() {
    return Sendjr({ {"t", "gni"} })["d"];
}
void inet::sendMsg(int type, Message m) {
    if (type == 0) {//text
        i.Send({ {'t', 'm'}, {'s', 't'}, {'n', m.sender}, {'i', m.time}, {'e', m.text}, {'u', m.uid}, {"d", m.id}, {'r', m.m} });
    }
    else if (type == 1) {//doc
        i.Send({ {'t', 'm'}, {'s', 'd'}, {'n', m.sender}, {'i', m.time}, {'b', m.doci}, {'u', m.uid}, {"d", m.id}, {'r', m.m} });
    }
    else if (type == 2) {//image
        i.Send({ {'t', 'm'}, {'s', 'i'}, {'n', m.sender}, {'i', m.time}, {'b', m.media64}, {'u', m.uid}, {"d", m.id}, {'r', m.m}, {'w', m.w}, {'h', m.h}});
    }
}
