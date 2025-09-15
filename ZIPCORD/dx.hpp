#pragma once
#define WIN32_LEAN_AND_MEAN  
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>    
#include <d3d10_1.h>
#include <d3d10.h>
#include <vector>
#include <string>

class DX {
public:
    ID3D10ShaderResourceView* CreateTextureFromMemory(ID3D10Device* device, const std::vector<uint8_t>& image_data);
    std::vector<uint8_t> Readfile(const std::wstring& filePath);
    std::vector<uint8_t> ReadPhoto(const std::wstring& filePath, int& width, int& height);
    std::string rezhava(std::string ava64);
    std::string GetFileType(const std::wstring& filePath);
    std::wstring GetFileType2(const std::wstring& filePath);
    std::string get_ava(int uid);
    void getWS(HWND& hwnd, float& x, float& y);
    std::string fileToBase64(const std::wstring& filePath);
    ID3D10ShaderResourceView* loadImage(ID3D10Device* device, const std::wstring& filePath);
    bool textureToBase64(ID3D10Device* device, ID3D10ShaderResourceView* textureId, std::string& base64Result, int& w, int& h);

private:

};
