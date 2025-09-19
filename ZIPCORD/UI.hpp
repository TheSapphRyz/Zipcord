#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <map>
#include "imgui.h"  
#include <string>


class UI {
public:
    void voicechat(HWND hwnd, float x, float y);
    static char buf[10000];
    void chat(HWND hwnd, float x, float y);
    void settingsw(HWND hwnd, float x, float y);
    void console(HWND hwnd, float x, float y);
    void registr(HWND hwnd, float x, float y);
    void acc(HWND hwnd, float x, float y);
    void react(HWND hwnd, float x, float y);
    void online(HWND hwnd, float x, float y);
    void auth(HWND hwnd, float x, float y);
    void sogll();
private:
    std::map<std::string, ImVec4> th_da; 
    std::map<std::string, ImVec4> th_de;
    std::map<std::string, ImVec4> th_li;
    std::map<std::string, ImVec4> th_x_;
    std::map<std::string, ImVec4> th_rb;
    std::map<std::string, ImVec4> th_pu;

};
