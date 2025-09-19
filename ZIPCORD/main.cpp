#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx10.h"
#include <d3d10_1.h>
#include <d3d10.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <string>
#include "portaudio.h"
#include <thread>
#include <atomic>
#include "json.hpp"

#include "font1.h" // roboto
#include "font2.h" // fa-solid-900
#include "fa6.hpp"
#include "google.hpp"
#include "kenney.hpp"
#include "lucide.hpp"
#include "IconsFontAwesome5.h"
#include "IconsGoogle.hpp"
#include "IconsKenney.hpp"
#include "IconsLucide.hpp"

#include <fstream>
#include <sstream>
#include <mutex>
#include <commdlg.h>
#include <map>
#include <wincrypt.h>
#include <cctype>
#include <ShlObj_core.h>
#include <ShObjIdl.h>
#include <algorithm>
#include <random>
#include <shellapi.h>
#include <winsqlite/winsqlite3.h>
#include <zlib.h>
#include "main.h"
#include "imgui_markdown.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "portaudio.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d3d10.lib")
#define U8(x) reinterpret_cast<const char*>(u8##x)
using json = nlohmann::json;
IDXGISwapChain* g_pSwapChain = nullptr;
bool g_SwapChainOccluded = false;
UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
ID3D10RenderTargetView* g_mainRenderTargetView = nullptr;
bool g_IsMinimized = false;
bool g_IsActive = true;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
std::vector<User> users_in_voice;
std::vector<Message> msgs;
std::vector<std::string>con_msgs;
bool shouldRender = false;
ID3D10Device* g_pd3dDevice = nullptr;
bool show_console = false;
// Определения экземпляров классов
DX dx;
inet i;
XOR_ xory;
Msg M;
UI ui;
AudioProcessor pa;
User u;
BD bd;
int main() {
    u.logined = true;
    u.regged = true;
    u.name = "ZIPCORD user";
    HMODULE hMod = LoadLibraryA("d3d10.dll");
    if (!hMod) {
        MessageBoxA(nullptr, "DirectX 10 is not supported on this system!", "Error", MB_ICONERROR);
        return 1;
    }
    FreeLibrary(hMod);
    i.initsocket();
    con_msgs.push_back("[INFO] Starting Internet");
    //users_in_voice = xory.split(i.Sendjr({ {"type", "guiv"} })["data"], "-=S=-");

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ZIPCORD", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"ZIPCORD DX10", WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);
    pa.start();
    setlocale(LC_ALL, "RU");
    if (!CreateDeviceD3D(hwnd)) {
        std::cerr << "Failed to create D3D device!" << std::endl;
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    con_msgs.push_back("[INFO] Starting DirectX");
    ::ShowWindow(GetConsoleWindow(), SW_SHOW);
    ::ShowWindow(hwnd, SW_SHOWNORMAL);
    ::UpdateWindow(hwnd);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    float x, y;
    dx.getWS(hwnd, x, y);
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX10_Init(g_pd3dDevice);
    float baseFontSize = y * 0.03;
    io.Fonts->Clear();
    io.Fonts->AddFontFromMemoryTTF((void*)font_ttf, font_ttf_size, baseFontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    io.ConfigDebugHighlightIdConflicts = false;
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = baseFontSize;
    io.Fonts->AddFontFromMemoryTTF((void*)fa6font, fa6font_size, baseFontSize, &icons_config, icons_ranges); //fa
    io.Fonts->AddFontFromMemoryTTF((void*)googlefont, googlefont_size, baseFontSize*0.8, &icons_config, icons_ranges); /// google
    io.Fonts->AddFontFromMemoryTTF((void*)kenneyfont, kenneyfont_size, baseFontSize, &icons_config, icons_ranges); // kenney
    io.Fonts->AddFontFromMemoryTTF((void*)lucidefont, lucidefont_size, baseFontSize, &icons_config, icons_ranges); // lucide
    ImVec4 clear_color = ImVec4(0.173f, 0.184f, 0.2f, 1.00f);
    con_msgs.push_back("[INFO] Fonts Loaded");
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    ImGui::StyleColorsDark();
    //style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ChildRounding = 6.0f;
    style.PopupRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.WindowBorderSize = 1.0f;
    style.ItemSpacing = ImVec2(8, 6);
    style.FramePadding = ImVec2(10, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.IndentSpacing = 20.0f;
    style.FrameBorderSize = 0.0f;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.ScrollbarSize = 10.0f; // Уменьшено для компактности
    style.GrabMinSize = 8.0f;

    colors[ImGuiCol_WindowBg] = COLOR_D_APP_BG; // Непрозрачный фон
    colors[ImGuiCol_Button] = COLOR_D_BTN;
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.30f, 0.35f, 1.00f); // Более заметный ховер
    colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.40f, 0.45f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_Text] = COLOR_TEXT; // Ярче текст
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigInputTrickleEventQueue = true;   
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    //io.Fonts->Build();
    //ImGui_ImplDX10_CreateDeviceObjects();
    bool done = false;
    if (!g_pd3dDevice || !g_pSwapChain || !g_mainRenderTargetView) printf("P");
    while (!done) {
        shouldRender = true;
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
            switch (msg.message) {
            case WM_MOUSEMOVE:
            case WM_NCMOUSEMOVE:
                break;
            default:
                break;
            }
        }
        if (done) break;
        if (g_IsMinimized || !g_IsActive) {
            ::Sleep(10);
            continue;
        }

        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) {
            ::Sleep(10);
            continue;
        }

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0) {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
            shouldRender = true;
        }
        float xx, yy;
        dx.getWS(hwnd, xx, yy);
        if (shouldRender) {
            ImGui_ImplDX10_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
            //if (ImGui::IsKeyPressed(ImGuiKey_F3)) show_console = !show_console;
            //if (show_console == true) ui.console(hwnd, xx, yy);
            if (showTermsWindow) ui.sogll();
            if (!u.regged) ui.registr(hwnd, xx, yy);
            else {
                ui.chat(hwnd, xx, yy);
                ui.voicechat(hwnd, xx, yy);
                ui.settingsw(hwnd, xx, yy);
            }
            ImGui::Render();
            const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
            g_pd3dDevice->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
            g_pd3dDevice->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
            ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
            HRESULT hr = g_pSwapChain->Present(1, 0);
            g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
            shouldRender = false;
        }
        else {
            ::Sleep(1);
        }
    }
    for (auto& msg : msgs) {
        if (msg.avaTexture) ((ID3D10ShaderResourceView*)msg.avaTexture)->Release();
        if (msg.mediaTexture) ((ID3D10ShaderResourceView*)msg.mediaTexture)->Release();
    }
    ImGui_ImplDX10_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    pa.stop();
    Pa_Terminate();
    i.stop();
	return 0;
}

bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

    HRESULT res = D3D10CreateDeviceAndSwapChain(
        nullptr,
        D3D10_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        D3D10_SDK_VERSION,
        &sd,
        &g_pSwapChain,
        &g_pd3dDevice
    );

    if (FAILED(res)) {
        // Попробуем создать устройство с WARP (программный рендерер)
        res = D3D10CreateDeviceAndSwapChain(
            nullptr,
            D3D10_DRIVER_TYPE_WARP,
            nullptr,
            createDeviceFlags,
            D3D10_SDK_VERSION,
            &sd,
            &g_pSwapChain,
            &g_pd3dDevice
        );

        if (FAILED(res)) {
            char buf[256];
            sprintf_s(buf, "Failed to create D3D10 device! HRESULT = 0x%08X", res);
            MessageBoxA(hWnd, buf, "Error", MB_ICONERROR);
            return false;
        }
    }

    CreateRenderTarget();
    return true;
}

/*bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    HRESULT res = D3D10CreateDeviceAndSwapChain(nullptr, D3D10_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, D3D10_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice);
    if (res == DXGI_ERROR_UNSUPPORTED) res = D3D10CreateDeviceAndSwapChain(nullptr, D3D10_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, D3D10_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice);
    if (res != S_OK) return false;

    CreateRenderTarget();
    return true;
}*/

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget() {
    ID3D10Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED) {
            g_IsMinimized = true;
            return 0;
        }
        else if (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED) {
            g_IsMinimized = false;
            g_ResizeWidth = (UINT)LOWORD(lParam);
            g_ResizeHeight = (UINT)HIWORD(lParam);
            shouldRender = true;
        }
        return 0;

    case WM_ACTIVATE:
        g_IsActive = (LOWORD(wParam) != WA_INACTIVE);
        if (g_IsActive) {
            shouldRender = true; 
        }
        return 0;

    case WM_ENTERSIZEMOVE:
    case WM_ENTERMENULOOP:
        SetTimer(hWnd, 1, 16, NULL);
        return 0;

    case WM_EXITSIZEMOVE:
    case WM_EXITMENULOOP:
        KillTimer(hWnd, 1);
        shouldRender = true;
        return 0;

    case WM_TIMER:
        if (wParam == 1) {
            shouldRender = true;
        }
        return 0;

    case WM_MOUSEMOVE:
        shouldRender = true;
    case WM_NCMOUSEMOVE:
        shouldRender = true;
        break;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Блокировка Alt-меню
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    if (msg >= WM_KEYFIRST && msg <= WM_IME_KEYLAST) {
        shouldRender = true;
    }

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
