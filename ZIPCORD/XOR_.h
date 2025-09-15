#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <windows.h>
#include <wincrypt.h>
#include <map>
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "windowscodecs.lib")

class XOR_ {
public:
    std::string XOR(std::string line, char type);
    std::string Base64Encode(const std::vector<uint8_t>& data);
    std::string base64_encode(const unsigned char* data, size_t length);
    std::vector<unsigned char> base64_decode(const std::string& encoded);
    std::vector<std::string> split(std::string s, std::string delimiter);
    std::string en(std::string text);
    std::string de(std::string text);
    std::string trim(const std::string& str);

private:
    static std::map<char, std::string> d;
    static std::string stb_key;
};