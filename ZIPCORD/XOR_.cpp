#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <wincrypt.h>
#include "XOR_.h"
#include <string>
#include <map>
#include <random>
#include <vector>
#include <zlib.h>
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "windowscodecs.lib")


XOR_ x;
std::map<char, std::string> XOR_::d = {
    {' ', "0000000"}, {'а', "0000001"}, {'б', "0000010"}, {'в', "0000011"}, {'г', "0000100"},
    {'д', "0000101"}, {'е', "0000110"}, {'ё', "0000111"}, {'ж', "0001000"}, {'з', "0001001"},
    {'и', "0001010"}, {'й', "0001011"}, {'к', "0001100"}, {'л', "0001101"}, {'м', "0001110"},
    {'н', "0001111"}, {'о', "0010000"}, {'п', "0010001"}, {'р', "0010010"}, {'с', "0010011"},
    {'т', "0010100"}, {'у', "0010101"}, {'ф', "0010110"}, {'х', "0010111"}, {'ц', "0011000"},
    {'ч', "0011001"}, {'ш', "0011010"}, {'щ', "0011011"}, {'ъ', "0011100"}, {'ы', "0011101"},
    {'ь', "0011110"}, {'э', "0011111"}, {'ю', "0100000"}, {'я', "0100001"}, {'a', "0100010"},
    {'b', "0100011"}, {'c', "0100100"}, {'d', "0100101"}, {'e', "0100110"}, {'f', "0100111"},
    {'g', "0101000"}, {'h', "0101001"}, {'i', "0101010"}, {'j', "0101011"}, {'k', "0101100"},
    {'l', "0101101"}, {'m', "0101110"}, {'n', "0101111"}, {'o', "0110000"}, {'p', "0110001"},
    {'q', "0110010"}, {'r', "0110011"}, {'s', "0110100"}, {'t', "0110101"}, {'u', "0110110"},
    {'v', "0110111"}, {'w', "0111000"}, {'x', "0111001"}, {'y', "0111010"}, {'z', "0111011"},
    {'0', "0111100"}, {'1', "0111101"}, {'2', "0111110"}, {'3', "0111111"}, {'4', "1000000"},
    {'5', "1000001"}, {'6', "1000010"}, {'7', "1000011"}, {'8', "1000100"}, {'9', "1000101"},
    {'@', "1000110"}, {'#', "1000111"}, {'_', "1001000"}, {'&', "1001001"}, {'-', "1001010"},
    {'+', "1001011"}, {'(', "1001100"}, {')', "1001101"}, {'/', "1001110"}, {'*', "1001111"},
    {'\"', "1010000"}, {'\'', "1010001"}, {':', "1010010"}, {';', "1010011"}, {'!', "1010100"},
    {'?', "1010101"}, {'~', "1010110"}, {'|', "1010111"}, {'$', "1011000"}, {'^', "1011001"},
    {'°', "1011010"}, {'=', "1011011"}, {'{', "1011100"}, {'}', "1011101"}, {'\\', "1011110"},
    {'%', "1011111"}, {'[', "1100000"}, {']', "1100001"}, {'<', "1100010"}, {'>', "1100011"},
    {',', "1100100"}, {'.', "1100101"}, {'A', "1100110"}, {'B', "1100111"}, {'C', "1101000"},
    {'D', "1101001"}, {'E', "1101010"}, {'F', "1101011"}, {'G', "1101100"}, {'H', "1101101"},
    {'I', "1101110"}, {'J', "1101111"}, {'K', "1110000"}, {'L', "1110001"}, {'M', "1110010"},
    {'N', "1110011"}, {'O', "1110100"}, {'P', "1110101"}, {'Q', "1110110"}, {'R', "1110111"},
    {'S', "1111000"}, {'T', "1111001"}, {'U', "1111010"}, {'V', "1111011"}, {'W', "1111100"},
    {'X', "1111101"}, {'Y', "1111110"}, {'Z', "1111111"}
};
std::string XOR_::stb_key = "JakaKustOrtemLexa";


std::string XOR_::XOR(std::string line, char type) {
        //<r
    std::string key;
    std::vector<char> keys;
    for (const auto& p : d) keys.push_back(p.first);

    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> dist(0, 127);

    for (int i = 0; i < line.size(); i++) key = key + keys[dist(e)];
        //r>
        //<r1
    std::random_device r1;
    std::default_random_engine e1(r1());
    std::uniform_int_distribution<int> dist1(1, 4);
    int mc = dist1(e1);
        //r1>



    if (type == 'e') {
        std::string mp;
        mp.resize(line.size());
        for (size_t i = 0; i < line.size(); i++) mp[i] = line[i] ^ key[i % key.size()];
        std::string g;
        std::string s;
        std::string go;
        
        if (mc == 1) {
            if (mp.size() % 2 != 0) {
                for (size_t i = 0; i < mp.size(); i += 1) {
                    g += mp.substr(i, 1);
                    if (i < key.size()) {
                        g += key.substr(i, 1);
                    }
                }
                s = "1";
            }
            else {
                for (size_t i = 0; i < mp.size(); i += 2) {
                    g += mp.substr(i, 2);
                    if (i < key.size()) {
                        g += key.substr(i, 2);
                    }
                }
                s = "2";
            }
            std::string ii = '1' + s + g;
            go.resize(ii.size());
            for (size_t i = 0; i < ii.size(); i++) go[i] = ii[i] ^ stb_key[i % stb_key.size()];
            return go;
        }
        else if (mc == 2) {
            std::reverse(mp.begin(), mp.end());
            if (mp.size() % 2 != 0) {
                for (size_t i = 0; i < mp.size(); i += 1) {
                    g += mp.substr(i, 1);
                    if (i < key.size()) {
                        g += key.substr(i, 1);
                    }
                }
                s = "1";

            }
            else {
                for (size_t i = 0; i < mp.size(); i += 2) {
                    g += mp.substr(i, 2);
                    if (i < key.size()) {
                        g += key.substr(i, 2);
                    }
                }
                s = "2";
            }
            std::string ii = '2' + s + g;
            go.resize(ii.size());
            for (size_t i = 0; i < ii.size(); i++) go[i] = ii[i] ^ stb_key[i % stb_key.size()];
            return go;
        }
        else if (mc == 3) {
            std::reverse(mp.begin(), mp.end());

            std::string ii = "30" + key + mp;
            go.resize(ii.size());
            for (size_t i = 0; i < ii.size(); i++) go[i] = ii[i] ^ stb_key[i % stb_key.size()];
            return go;
        }
        else if (mc == 4) {
            std::reverse(key.begin(), key.end());

            std::string ii = "40" + mp + key;
            go.resize(ii.size());
            for (size_t i = 0; i < ii.size(); i++) go[i] = ii[i] ^ stb_key[i % stb_key.size()];
            return go;
        }
    }
    else if (type == 'd') {
        std::string mp;
        mp.resize(line.size());
        std::string go;

        for (size_t i = 0; i < line.size(); i++) mp[i] = line[i] ^ stb_key[i % stb_key.size()];

        if (mp[0] == '1') {
            if (mp[1] == '1') {
                std::string m, k;
                std::string p = mp.erase(0, 2);

                for (size_t i = 0; i < p.size(); i += 2) {
                    m += p.substr(i, 1);
                    k += p.substr(i + 1, 1);
                }
                go.resize(m.size());
                for (size_t i = 0; i < m.size(); i++) go[i] = m[i] ^ k[i % k.size()];
            }
            else if (mp[1] == '2') {
                std::string m, k;
                std::string p = mp.erase(0, 2);

                for (size_t i = 0; i < p.size(); i += 4) {
                    m += p.substr(i, 2);
                    k += p.substr(i + 2, 2);
                }
                go.resize(m.size());
                for (size_t i = 0; i < m.size(); i++) go[i] = m[i] ^ k[i % k.size()];

            }
        }
        else if (mp[0] == '2') {
            if (mp[1] == '1') {
                std::string m, k;
                std::string p = mp.erase(0, 2);

                for (size_t i = 0; i < p.size(); i += 2) {
                    m += p.substr(i, 1);
                    k += p.substr(i + 1, 1);
                }
                go.resize(m.size());
                std::reverse(m.begin(), m.end());
                for (size_t i = 0; i < m.size(); i++) go[i] = m[i] ^ k[i % k.size()];
            }
            else if (mp[1] == '2') {
                std::string m, k;
                std::string p = mp.erase(0, 2);
                for (size_t i = 0; i < p.size(); i += 4) {
                    m += p.substr(i, 2);
                    k += p.substr(i + 2, 2);
                }
                go.resize(m.size());
                std::reverse(m.begin(), m.end());
                for (size_t i = 0; i < m.size(); i++) go[i] = m[i] ^ k[i % k.size()];
            }

        }
        else if (mp[0] == '3') {
            std::string m, k;
            std::string p = mp.erase(0, 2);
            k = p.substr(0, p.size() / 2);
            m = p.substr(p.size() / 2);
            go.resize(m.size());
            std::reverse(m.begin(), m.end());
            for (size_t i = 0; i < m.size(); i++) go[i] = m[i] ^ k[i % k.size()];
        }
        else if (mp[0] == '4') {
            std::string m, k;
            std::string p = mp.erase(0, 2);
            k = p.substr(p.size() / 2);
            m = p.substr(0, p.size() / 2);
            go.resize(m.size());
            std::reverse(k.begin(), k.end());
            for (size_t i = 0; i < m.size(); i++) go[i] = m[i] ^ k[i % k.size()];
        }
        return go;
    }
}
std::string XOR_::en(std::string text) {
    uLong sourceLen = text.size();
    uLong destLen = compressBound(sourceLen);
    std::vector<unsigned char> buffer(destLen);
    int res = compress(buffer.data(), &destLen, reinterpret_cast<const Bytef*>(text.data()), sourceLen);
    if (res != Z_OK) throw std::runtime_error("Failed to compress data");
    return x.XOR(std::string(reinterpret_cast<char*>(buffer.data()), destLen), 'e'); // тут добавить, чтобы добавлялся размер исходного сообщения
}

std::string XOR_::de(std::string text) {
    std::string compressedStr = x.XOR(text, 'd');
    uLongf originalSize = static_cast<uLongf>(std::stoi(compressedStr)); // тут добавть нахождение в расшифрованном размер исходного
    std::vector<unsigned char> buffer(originalSize);
    int res = uncompress(buffer.data(), &originalSize, reinterpret_cast<const Bytef*>(compressedStr.data()), compressedStr.size());
    if (res != Z_OK) {
        throw std::runtime_error("Failed to decompress data");
    }
    return std::string(reinterpret_cast<char*>(buffer.data()), originalSize);
}
std::string XOR_::Base64Encode(const std::vector<uint8_t>& data) {
    DWORD base64Len = 0;
    if (!CryptBinaryToStringA(data.data(), data.size(), CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &base64Len)) throw std::runtime_error("Ошибка определения размера Base64!");
    std::string base64(base64Len, '\0');
    if (!CryptBinaryToStringA(data.data(), data.size(), CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, &base64[0], &base64Len)) throw std::runtime_error("Ошибка кодирования Base64!");
    base64.resize(base64Len - 1);
    return base64;
}
std::string XOR_::base64_encode(const unsigned char* data, size_t length) {
    static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    result.reserve((length + 2) / 3 * 4);
    for (size_t i = 0; i < length; i += 3) {
        unsigned char b1 = data[i];
        unsigned char b2 = (i + 1 < length) ? data[i + 1] : 0;
        unsigned char b3 = (i + 2 < length) ? data[i + 2] : 0;
        result.push_back(base64_chars[b1 >> 2]);
        result.push_back(base64_chars[((b1 & 0x03) << 4) | (b2 >> 4)]);
        result.push_back((i + 1 < length) ? base64_chars[((b2 & 0x0F) << 2) | (b3 >> 6)] : '=');
        result.push_back((i + 2 < length) ? base64_chars[b3 & 0x3F] : '=');
    }
    return result;
}
std::vector<unsigned char> XOR_::base64_decode(const std::string& encoded) {
    static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<unsigned char> result;
    result.reserve(encoded.length() * 3 / 4);
    std::vector<int> lookup(256, -1);
    for (int i = 0; i < 64; ++i) lookup[base64_chars[i]] = i;
    int val = 0, bits = -8;
    for (char c : encoded) {
       if (c == '=') break;
       if (lookup[c] == -1) continue;
       val = (val << 6) + lookup[c];
       bits += 6;
       if (bits >= 0) {
           result.push_back((val >> bits) & 0xFF);
           bits -= 8;
       }
    }
    return result;
}

std::vector<std::string> XOR_::split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

std::string XOR_::trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
        return std::isspace(ch);
        });

    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
        return std::isspace(ch);
        }).base();

    return (start < end) ? std::string(start, end) : "";
}