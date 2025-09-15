#pragma once
#include "json.hpp"
using json = nlohmann::json;

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <Windows.h>
#include "main.h"
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
struct Message;
class inet {
public:
    void initsocket();
    void recv_();
    void reconnect();
    void Send(json data);
    json Sendjr(json request);
    SOCKET getcls();
    void stop();
    int getnewid();
    void sendMsg(int type, Message m);
private:
    static SOCKET cl;
};