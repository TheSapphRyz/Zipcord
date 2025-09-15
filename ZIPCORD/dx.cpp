#include "DX.hpp"
#define _CRT_SECURE_NO_WARNINGS
#include <d3d10_1.h>
#include <d3d10.h>
#include <vector>
#include <string>
#include <wincodec.h>
#include "json.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "main.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d3d10_1.lib")

ID3D10ShaderResourceView* DX::CreateTextureFromMemory(ID3D10Device* device, const std::vector<uint8_t>& image_data) {
    if (image_data.empty() || !device) return nullptr;
    int width, height, channels;
    if (!stbi_info_from_memory(image_data.data(), static_cast<int>(image_data.size()), &width, &height, &channels)) return nullptr;
    uint8_t* pixels = stbi_load_from_memory(image_data.data(), static_cast<int>(image_data.size()), &width, &height, &channels, 4);
    if (!pixels) return nullptr;
    D3D10_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D10_USAGE_DEFAULT;
    texDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    D3D10_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixels;
    initData.SysMemPitch = width * 4;
    initData.SysMemSlicePitch = 0;
    ID3D10Texture2D* texture = nullptr;
    HRESULT hr = device->CreateTexture2D(&texDesc, &initData, &texture);
    stbi_image_free(pixels);
    if (FAILED(hr) || !texture) return nullptr;
    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    ID3D10ShaderResourceView* textureView = nullptr;
    hr = device->CreateShaderResourceView(texture, &srvDesc, &textureView);
    texture->Release();
    if (FAILED(hr)) return nullptr;
    return textureView;
}
std::vector<uint8_t> DX::Readfile(const std::wstring& filePath) {
    DWORD fileAttributes = GetFileAttributesW(filePath.c_str());
    if (fileAttributes == INVALID_FILE_ATTRIBUTES || (fileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        throw std::runtime_error("File doesn't exist or is a directory!");

    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Failed to open file! Error code: " + std::to_string(GetLastError()));

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        CloseHandle(hFile);
        throw std::runtime_error("Failed to get file size!");
    }

    std::vector<uint8_t> buffer(fileSize.QuadPart);
    DWORD bytesRead = 0;

    CloseHandle(hFile);
    return buffer;
}
std::vector<uint8_t> DX::ReadPhoto(const std::wstring& filePath, int& width, int& height) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), (int)filePath.size(), NULL, 0, NULL, NULL);
    std::string narrowPath(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), (int)filePath.size(), narrowPath.data(), size_needed, NULL, NULL);
    int bpp;
    unsigned char* data = stbi_load(narrowPath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    std::vector<uint8_t> imageData(data, data + width * height * STB_CH);
    return imageData;
    stbi_image_free(data);
}
std::string DX::rezhava(std::string ava64) {
    std::vector<unsigned char> imageData = xory.base64_decode(ava64);
    if (imageData.empty()) return "";

    int width, height, channels;
    unsigned char* img = stbi_load_from_memory(imageData.data(), (int)imageData.size(), &width, &height, &channels, 4);
    if (!img) return "";
    int squareSize = (((width) < (height)) ? (width) : (height));
    int xOffset = (width - squareSize) / 2;
    int yOffset = (height - squareSize) / 2;
    std::vector<unsigned char> cropped(squareSize * squareSize * 4);
    for (int y = 0; y < squareSize; y++) {
        memcpy(&cropped[y * squareSize * 4],
            &img[((y + yOffset) * width + xOffset) * 4],
            squareSize * 4);
    }

    stbi_image_free(img);
    std::vector<unsigned char> pngBuffer;
    auto WriteToMemory = [](void* context, void* data, int size) {
        std::vector<unsigned char>* buffer = (std::vector<unsigned char>*)context;
        unsigned char* d = (unsigned char*)data;
        buffer->insert(buffer->end(), d, d + size);
        };
    int res = stbi_write_png_to_func(WriteToMemory, &pngBuffer, squareSize, squareSize, 4, cropped.data(), squareSize * 4);
    if (!res) return "";
    return xory.base64_encode(pngBuffer.data(), pngBuffer.size());
}
std::string DX::GetFileType(const std::wstring& filePath) {
    std::wstring ext = filePath.substr(filePath.find_last_of(L".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    const std::map<std::wstring, std::string> typeMap = {
        {L"jpg", "image"},
        {L"jpeg", "image"},
        {L"png", "image"},
        {L"gif", "video"},
        {L"bmp", "image"},
        {L"mp4", "video"},
        {L"avi", "video"},
        {L"mov", "video"},
        {L"mkv", "video"},
        {L"webm", "video"},
    };

    auto it = typeMap.find(ext);
    return (it != typeMap.end()) ? it->second : "doc";
}

std::wstring DX::GetFileType2(const std::wstring& filePath) {
    std::wstring ext = filePath.substr(filePath.find_last_of(L".") + 1);
    return ext;
}
std::string DX::get_ava(int uid) {
    json r = { {"type", "getava"}, {"uid", uid} };
    json a = i.Sendjr(r);
    return a["data"];
}

void DX::getWS(HWND& hwnd, float& x, float& y) {
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    x = windowRect.right - windowRect.left;
    y = windowRect.bottom - windowRect.top;
}
std::string DX::fileToBase64(const std::wstring& filePath) {
    // Открываем файл
    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return "";
    }

    // Получаем размер файла
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize) || fileSize.QuadPart == 0) {
        CloseHandle(hFile);
        return "";
    }

    // Буфер для чтения (64 КБ)
    const size_t chunkSize = 64 * 1024;
    std::vector<char> buffer(chunkSize);
    std::string base64Result;
    base64Result.reserve(static_cast<size_t>(fileSize.QuadPart * 4 / 3 + 3)); // Резервируем память для base64

    DWORD bytesRead;
    while (ReadFile(hFile, buffer.data(), chunkSize, &bytesRead, NULL) && bytesRead > 0) {
        // Кодируем кусок в base64
        std::string encoded = xory.base64_encode(reinterpret_cast<const unsigned char*>(buffer.data()), bytesRead);
        base64Result.append(encoded);
    }

    CloseHandle(hFile);
    return base64Result;
}


ID3D10ShaderResourceView* DX::loadImage(ID3D10Device* device, const std::wstring& filePath) {
    HRESULT hr;
    IWICImagingFactory* factory = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;

    // Инициализация COM
    CoInitialize(NULL);

    // Создание WIC фабрики
    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        CoUninitialize();
        return nullptr;
    }

    // Загрузка изображения
    hr = factory->CreateDecoderFromFilename(filePath.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
    if (FAILED(hr)) {
        factory->Release();
        CoUninitialize();
        return nullptr;
    }

    // Получение первого кадра
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) {
        decoder->Release();
        factory->Release();
        CoUninitialize();
        return nullptr;
    }

    // Получение размеров изображения
    UINT width, height;
    hr = frame->GetSize(&width, &height);
    if (FAILED(hr)) {
        frame->Release();
        decoder->Release();
        factory->Release();
        CoUninitialize();
        return nullptr;
    }

    // Конвертация в 32bpp BGRA
    hr = factory->CreateFormatConverter(&converter);
    if (FAILED(hr)) {
        frame->Release();
        decoder->Release();
        factory->Release();
        CoUninitialize();
        return nullptr;
    }

    hr = converter->Initialize(frame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) {
        converter->Release();
        frame->Release();
        decoder->Release();
        factory->Release();
        CoUninitialize();
        return nullptr;
    }

    // Копирование данных пикселей
    UINT stride = width * 4; // 4 байта на пиксель (BGRA)
    UINT bufferSize = stride * height;
    std::vector<BYTE> buffer(bufferSize);
    hr = converter->CopyPixels(NULL, stride, bufferSize, buffer.data());
    if (FAILED(hr)) {
        converter->Release();
        frame->Release();
        decoder->Release();
        factory->Release();
        CoUninitialize();
        return nullptr;
    }

    // Создание DirectX 10 текстуры
    D3D10_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D10_USAGE_DEFAULT;
    desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;

    D3D10_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = buffer.data();
    initData.SysMemPitch = stride;

    ID3D10Texture2D* texture = nullptr;
    hr = device->CreateTexture2D(&desc, &initData, &texture);
    if (FAILED(hr)) {
        converter->Release();
        frame->Release();
        decoder->Release();
        factory->Release();
        CoUninitialize();
        return nullptr;
    }

    // Создание Shader Resource View для ImGui
    ID3D10ShaderResourceView* srv = nullptr;
    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    hr = device->CreateShaderResourceView(texture, &srvDesc, &srv);
    texture->Release(); // Освобождаем текстуру, так как SRV достаточно для ImGui

    // Очистка WIC ресурсов
    converter->Release();
    frame->Release();
    decoder->Release();
    factory->Release();
    CoUninitialize();

    if (FAILED(hr)) {
        return nullptr;
    }

    return srv;
}
bool DX::textureToBase64(ID3D10Device* device, ID3D10ShaderResourceView* textureId, std::string& base64Result, int& w, int& h) {
    ID3D10ShaderResourceView* srv = textureId;
    ID3D10Resource* resource = nullptr;
    srv->GetResource(&resource);
    if (!resource) {
        return false;
    }

    // Приведение к ID3D10Texture2D
    ID3D10Texture2D* texture = nullptr;
    HRESULT hr = resource->QueryInterface(__uuidof(ID3D10Texture2D), (void**)&texture);
    resource->Release();
    if (FAILED(hr) || !texture) {
        return false;
    }

    // Получение описания текстуры
    D3D10_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);
    if (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM) {
        texture->Release();
        return false; // Поддерживается только BGRA формат
    }

    // Запись размеров
    w = desc.Width;
    h = desc.Height;

    // Создание staging текстуры для чтения данных
    D3D10_TEXTURE2D_DESC stagingDesc = desc;
    stagingDesc.Usage = D3D10_USAGE_STAGING;
    stagingDesc.BindFlags = 0;
    stagingDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;

    ID3D10Texture2D* stagingTexture = nullptr;
    hr = device->CreateTexture2D(&stagingDesc, nullptr, &stagingTexture);
    if (FAILED(hr)) {
        texture->Release();
        return false;
    }

    // Копирование данных текстуры в staging текстуру
    device->CopyResource(stagingTexture, texture);
    texture->Release();

    // Маппинг staging текстуры для чтения данных
    D3D10_MAPPED_TEXTURE2D mapped;
    hr = stagingTexture->Map(0, D3D10_MAP_READ, 0, &mapped);
    if (FAILED(hr)) {
        stagingTexture->Release();
        return false;
    }

    // Копирование данных пикселей
    UINT stride = desc.Width * 4; // 4 байта на пиксель (BGRA)
    UINT bufferSize = stride * desc.Height;
    std::vector<BYTE> buffer(bufferSize);
    for (UINT i = 0; i < desc.Height; ++i) {
        memcpy(buffer.data() + i * stride, (BYTE*)mapped.pData + i * mapped.RowPitch, stride);
    }

    stagingTexture->Unmap(0);
    stagingTexture->Release();

    // Кодирование данных в base64
    base64Result = xory.base64_encode(buffer.data(), bufferSize);
    return true;
}