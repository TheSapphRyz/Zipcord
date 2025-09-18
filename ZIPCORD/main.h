// main.h
#pragma once
#include "inet.h"
#include "XOR_.h"
#include "AudioProcessor.hpp"
#include "MSG.hpp"
#include "DX.hpp"
#include "UI.hpp"
#include "BD.hpp"
#include "colors.hpp"
#include <vector>

#define SAMPLE_RATE 48000
#define FRAMES_PER_BUFFER 960
#define NUM_CHANNELS 1
#define STB_CH 4
#define MAX_PACKET_SIZE 4000    

extern std::vector<std::string> users_in_voice;

struct Message {
    std::string text;
    std::string sender;
    std::string time;
    int id;
    int uid;
    int cid;
    std::string media64;
    ImTextureID avaTexture;
    ImTextureID mediaTexture;
    bool isImage;
    bool isText;
    bool doc;
    int doci; // это did и если надо скачать - запрос на сервер // название файла будет uid_mid
    int w;
    int h;
    int reply;
    std::map<int, std::string> m; // rid (reaction id) и "Sosikust_Zyd" - это и те кто нажал, если ты там - кнопка другого цвета, количество нажатий на нее
};
struct Chat {
    std::string name;
    std::string ava64;
    ImTextureID ava;
    std::string last_msg_w_time;
    int um; // количество непрочитанных
    int cid;
    int par; // количество людей в группе
    bool ls_or_group;

};
struct User {
    std::string name;
    int uid;
    ImTextureID avaTexture;
    std::string theme_gl;
    bool mic;
    bool invoice;
    bool logined;
    bool regged;
};
extern bool showTermsWindow;
extern User u;
extern std::vector<Message> msgs;
extern std::vector<std::string> con_msgs;
extern bool shouldRender;
extern ID3D10Device* g_pd3dDevice;

// Объявления классов
extern class DX dx;
extern class inet i;
extern class XOR_ xory;
extern class Msg M;
extern class UI ui;
extern class AudioProcessor pa;
extern class BD bd;