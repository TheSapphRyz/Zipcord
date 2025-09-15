#pragma once
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>   
#include <Windows.h>
#include <vector>
#include <ws2tcpip.h>   


class Msg {
public:
    bool SaveFileToDownloads(const std::vector<uint8_t>& fileData, const std::string& fileName);
    std::string getTime();
    int getIdMsg();
private:

};