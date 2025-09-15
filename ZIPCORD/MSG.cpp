#include "MSG.hpp"
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include "main.h"
#include <ShlObj_core.h>
#include <ShObjIdl.h>
#pragma comment(lib, "Ws2_32.lib")

bool Msg::SaveFileToDownloads(const std::vector<uint8_t>& fileData, const std::string& fileName) {
    PWSTR downloadsPathW = nullptr;
    if (SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &downloadsPathW) != S_OK) return false;
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &fileName[0], (int)fileName.size(), NULL, 0);
    std::wstring fileNameW(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &fileName[0], (int)fileName.size(), &fileNameW[0], size_needed);
    std::wstring fullPath = std::wstring(downloadsPathW) + L"\\" + fileNameW;
    CoTaskMemFree(downloadsPathW);
    std::ofstream file(fullPath, std::ios::binary);
    if (!file.is_open()) return false;
    file.write(reinterpret_cast<const char*>(fileData.data()), fileData.size());
    return file.good();
}
std::string Msg::getTime() {
    SYSTEMTIME st;
    GetSystemTime(&st);
    char t[50] = "";
    sprintf_s(t, "%d:%d", st.wHour + 3, st.wMinute);
    return std::string(t);
}
int Msg::getIdMsg() {return std::stoi(std::string(i.Sendjr({ {"type", "gni"} })["data"]));}