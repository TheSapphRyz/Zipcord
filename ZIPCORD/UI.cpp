#include "UI.hpp"
#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx10.h"
#include "main.h"
#include <string>
#include "IconsFontAwesome5.h"
#include "IconsGoogle.hpp"
#include "IconsKenney.hpp"
#include "IconsLucide.hpp"
#include <fstream>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <shellapi.h>
#pragma comment(lib, "comdlg32.lib")
#include <commdlg.h>
#include "imgui_markdown.h"
#include <functional>
#include <regex>    
#define U8(x) reinterpret_cast<const char*>(u8##x)
static ImGui::MarkdownConfig mdConfig;
void LinkCallback(ImGui::MarkdownLinkCallbackData data_)
{
    std::string url(data_.link, data_.linkLength);
    if (!data_.isImage)
    {
        ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
}
void ExampleMarkdownFormatCallback(const ImGui::MarkdownFormatInfo& markdownFormatInfo_, bool start_)
{
    // Call the default first so any settings can be overwritten by our implementation.
    // Alternatively could be called or not called in a switch statement on a case by case basis.
    // See defaultMarkdownFormatCallback definition for furhter examples of how to use it.
    ImGui::defaultMarkdownFormatCallback(markdownFormatInfo_, start_);

    switch (markdownFormatInfo_.type)
    {
        // example: change the colour of heading level 2
    case ImGui::MarkdownFormatType::HEADING:
    {
        if (markdownFormatInfo_.level == 2)
        {
            if (start_){ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);}
            else{ImGui::PopStyleColor();}
        }
        break;
    }
    default:
    {
        break;
    }
    }
}
void Markdown(const std::string& markdown_)
{
    // You can make your own Markdown function with your prefered string container and markdown config.
    // > C++14 can use ImGui::MarkdownConfig mdConfig{ LinkCallback, NULL, ImageCallback, ICON_FA_LINK, { { H1, true }, { H2, true }, { H3, false } }, NULL };
    mdConfig.linkCallback = LinkCallback;
    mdConfig.tooltipCallback = NULL;
    mdConfig.linkIcon = ICON_FA_LINK;
    mdConfig.headingFormats[0] = { NULL, false }; // Отключаем заголовки
    mdConfig.headingFormats[1] = { NULL, false };
    mdConfig.headingFormats[2] = { NULL, false };

    mdConfig.userData = NULL;
    mdConfig.formatCallback = ExampleMarkdownFormatCallback;
    ImGui::Markdown(markdown_.c_str(), markdown_.length(), mdConfig);
}

void UI::voicechat(HWND hwnd, float x, float y) {
    if (!u.invoice) {
        ImGui::Begin("Voice", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
        ImGui::SetWindowSize(ImVec2(x * 0.4, y * 0.7));
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImVec2 bs = ImVec2(ImGui::CalcTextSize(reinterpret_cast<const char*>(u8"Подключиться")).x + 10, ImGui::CalcTextSize(reinterpret_cast<const char*>(u8"Подключиться")).y + 4);
        ImGui::SetCursorPos(ImVec2(((x*0.4)/2)-(bs.x/2), y*0.1));
        if (ImGui::Button(reinterpret_cast<const char*>(u8"Подключиться"), ImVec2(bs.x, bs.y))) {
            i.Send({ {"type", "cv"}, {"name", u.name} });
            u.invoice = !u.invoice;
        }
        //ImGui::Text("Users in voice chat: ");
        Markdown(reinterpret_cast<const char*>(u8"Людей в войс чате: "));
        ImGui::SameLine();
        ImGui::Text(std::to_string(users_in_voice.size()).c_str());
        ImGui::End();

    }
    else {
        ImGui::Begin("Voice", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
        ImGui::SetWindowSize(ImVec2(x * 0.4, y * 0.7));
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetCursorPos(ImVec2(ImGui::CalcTextSize(reinterpret_cast<const char*>(u8"Войс чат")).x + 10, ImGui::CalcTextSize(reinterpret_cast<const char*>(u8"Войс чат")).y + 4));
        ImGui::Text(reinterpret_cast<const char*>(u8"войс чат"));

        if (users_in_voice.size() > 0) {
            for (std::string i : users_in_voice) {
                ImGui::Text(i.c_str());
            }
        }

        ImGui::SetCursorPos(ImVec2(x * 0.2 - x * 0.03, y * 0.6 - x * 0.04));
        if (u.mic) {
            if (ImGui::Button(ICON_FA_MICROPHONE, ImVec2(x * 0.03, x * 0.03))) {
                u.mic = !u.mic;
            }
        }
        else if (!u.mic) {
            if (ImGui::Button(ICON_FA_MICROPHONE_SLASH, ImVec2(x * 0.03, x * 0.03))) {
                u.mic = !u.mic;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_PHONE, ImVec2(x * 0.03, x * 0.03))) {
            u.invoice = !u.invoice;
        }
        ImGui::End();

    }
}
void SendNotification(const std::string& username) {
    printf("@ti");
}

// Функция для рендеринга colored и wrapped текста
void RenderColoredWrappedText(const std::string& text, float wrapWidth) {
    std::regex mention_regex(R"((?<=\s|^)@(\d+))");
    std::sregex_iterator iter(text.begin(), text.end(), mention_regex);
    std::sregex_iterator end;

    std::vector<std::pair<size_t, size_t>> mention_positions;
    for (; iter != end; ++iter) {
        mention_positions.emplace_back(iter->position(), iter->length());
    }

    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrapWidth);
    size_t last_pos = 0;
    for (const auto& pos : mention_positions) {
        if (pos.first > last_pos) {
            std::string normal_text = text.substr(last_pos, pos.first - last_pos);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::TextUnformatted(normal_text.c_str());
            ImGui::PopStyleColor();
            ImGui::SameLine(0.0f, 0.0f);
        }
        std::string mention_text = text.substr(pos.first, pos.second);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.5f, 1.0f, 1.0f));
        ImGui::TextUnformatted(mention_text.c_str());
        ImGui::PopStyleColor();
        ImGui::SameLine(0.0f, 0.0f);
        last_pos = pos.first + pos.second;
    }
    if (last_pos < text.length()) {
        std::string normal_text = text.substr(last_pos);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::TextUnformatted(normal_text.c_str());
        ImGui::PopStyleColor();
    }
    ImGui::PopTextWrapPos();
}
char UI::buf[10000];
void UI::chat(HWND hwnd, float x, float y) {
    ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowSize(ImVec2(x * 0.6, y));
    ImGui::SetWindowPos(ImVec2(x * 0.4, 0));
    ImGui::BeginChild("ChatScroll", ImVec2(x * 0.6, y - 100), ImGuiWindowFlags_AlwaysVerticalScrollbar);
    float scrollY = ImGui::GetScrollY();
    float wH = ImGui::GetWindowHeight();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float cursorY = 0.0f;
    int idid;
    for (Message msgi : msgs) {
        double itemH;
        if (msgi.isImage) {
            int w, h;
            w = x * 0.3 - 10;
            h = (w * msgi.h) / msgi.w;        if (h > y) {
                h = y;
                w = (h * msgi.w) / msgi.h;
            }
            std::string header = msgi.sender + "  " + msgi.time;
            ImVec2 hSize = ImGui::CalcTextSize(header.c_str(), nullptr, false, x * 0.54f);
            itemH = hSize.y + msgi.h + 40;
        }
        else if (msgi.doc) itemH = x * 0.06;
        else if (msgi.isText) {
            ImVec2 textSize = ImGui::CalcTextSize(msgi.text.c_str(), nullptr, false, x * 0.54f);
            itemH = textSize.y + 50.0f;
        }
        if (cursorY + itemH >= scrollY && cursorY <= scrollY + wH) {
            ImVec2 pos = ImGui::GetCursorScreenPos();
            pos.y += cursorY;

            if (msgi.isImage) {
                int w, h;
                w = x * 0.3 - 10;
                h = (w * msgi.h) / msgi.w;

                if (h > y) {
                    h = y;
                    w = (h * msgi.w) / msgi.h;
                }
                ImVec2 avaPos = ImVec2(pos.x, pos.y + itemH - x * 0.015);
                draw_list->AddImage(msgi.avaTexture, avaPos, ImVec2(avaPos.x + x * 0.015, avaPos.y + y * 0.015));
                std::string header = msgi.sender + "  " + msgi.time;
                ImVec2 hSize = ImGui::CalcTextSize(header.c_str(), nullptr, false, x * 0.54f);
                float boxWidth = (hSize.x > msgi.w ? hSize.x : msgi.w) + 20;
                ImVec2 msgPos = ImVec2(pos.x + x * 0.015 + 8, pos.y);
                draw_list->AddRectFilled(msgPos, ImVec2(msgPos.x + boxWidth - x * 0.015 - 8, msgPos.y + itemH - 5), ImGui::GetColorU32(ImGuiCol_ChildBg), 12.0f);
                ImGui::SetCursorScreenPos(ImVec2(msgPos.x + 10, msgPos.y + 10));
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), header.c_str());
                ImGui::Separator();
                ImGui::Image((ImTextureID)dx.CreateTextureFromMemory(g_pd3dDevice, xory.base64_decode(msgi.media64)), ImVec2(w, h));
                ImVec2 btndp = ImVec2(pos.x + boxWidth + 8, pos.y + itemH - x * 0.02);
                ImGui::SetCursorPos(btndp);
                if (ImGui::Button(ICON_MD_DOWNLOAD, ImVec2(x * 0.02, x * 0.02))) {
                    
                }
                ImGui::SetCursorPos(ImVec2(btndp.x, btndp.y - x * 0.02 - 5));
                if (ImGui::Button(ICON_FA_HEART, ImVec2(x * 0.02, x * 0.02))) {

                }
            }
            else if (msgi.doc) {
                ImVec2 avaPos = ImVec2(pos.x, pos.y + itemH - x * 0.015);
                draw_list->AddImage(msgi.avaTexture, avaPos, ImVec2(avaPos.x + x * 0.015, avaPos.y + y * 0.015));
                std::string header = msgi.sender + "  " + msgi.time;
                ImVec2 hSize = ImGui::CalcTextSize(header.c_str(), nullptr, false, x * 0.54f);

                ImVec2 msgPos = ImVec2(pos.x + x * 0.015 + 8, pos.y);
                draw_list->AddRectFilled(msgPos, ImVec2(msgPos.x + hSize.x - x * 0.015 - 8, msgPos.y + itemH - 5), ImGui::GetColorU32(ImGuiCol_ChildBg), 12.0f);
                ImGui::SetCursorScreenPos(ImVec2(msgPos.x + 10, msgPos.y + 10));
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), header.c_str());
                ImGui::Separator();
                ImVec2 btndp = ImVec2(pos.x + hSize.x + 8, pos.y + itemH - x * 0.02);
                ImGui::SetCursorPos(btndp);
                if (ImGui::Button(ICON_FA_DOWNLOAD, ImVec2(x * 0.02, x * 0.02))) {
                    
                }
                ImGui::SetCursorPos(ImVec2(btndp.x, btndp.y - x * 0.02 - 5));
                if (ImGui::Button(ICON_FA_HEART, ImVec2(x * 0.02, x * 0.02))) {

                }
            }
            else if (msgi.isText) {
                ImVec2 avaPos = ImVec2(pos.x, pos.y + itemH - x * 0.015);
                draw_list->AddImage(msgi.avaTexture, avaPos, ImVec2(avaPos.x + x * 0.015, avaPos.y + y * 0.015));
                std::string header = msgi.sender + "  " + msgi.time;
                ImVec2 hSize = ImGui::CalcTextSize(header.c_str(), nullptr, false, x * 0.54f);
                ImVec2 textSize = ImGui::CalcTextSize(msgi.text.c_str(), nullptr, false, x * 0.54f);
                float minWidth = 100.0f;
                float maxWidth = x * 0.58f;
                float boxWidth;
                if (hSize.x >= textSize.x) {
                    boxWidth = hSize.x + 20;
                }
                else {
                    boxWidth = textSize.x + 20;
                }
                ImVec2 msgPos = ImVec2(pos.x + x * 0.015 + 8, pos.y);
                ImVec2 msgRectEnd = ImVec2(msgPos.x + boxWidth, msgPos.y + itemH);
                draw_list->AddRectFilled(msgPos, msgRectEnd, ImGui::GetColorU32(ImVec4(0.322, 0.322, 0.322, 1.0)), 12.0f);
                ImGui::SetCursorPos(ImVec2(msgPos.x + boxWidth, msgPos.y));
                //if (ImGui::Button(ICON_FA_HEART, ImVec2(x * 0.02, x * 0.02))) {

                //}
                ImGui::SetCursorScreenPos(ImVec2(msgPos.x + 10, msgPos.y + 10));
                ImGui::TextColored(ImVec4(0.6, 0.667, 0.71, 1.0f), header.c_str());
                //ImGui::SetCursorScreenPos(ImVec2(msgPos.x + 10, msgPos.y + 10 + hSize.y + 2));
                //ImGui::Separator();
                ImGui::SetCursorScreenPos(ImVec2(msgPos.x + 10, msgPos.y + 10 + hSize.y + 8));
                ImGui::PushTextWrapPos(msgPos.x + boxWidth - 10);
                ImGui::TextUnformatted(msgi.text.c_str());
                ImGui::PopTextWrapPos();
            }
        }
    }

    ImGui::EndChild();

    float inputPanelHeight = y*0.07; // Фиксированная высота панели
    ImGui::SetCursorPos(ImVec2((x*0.6-x*0.46)/2, y - inputPanelHeight - 20.0f)); // Отступ 20 пикселей от края
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.125f, 0.133f, 0.145f, 1.0f));
    ImGui::BeginChild("InputPanel", ImVec2(x * 0.46, inputPanelHeight+y*0.02), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    float baseFontSize = std::clamp(x * 0.015f, 12.0f, 24.0f);
    float iconSize = baseFontSize * 1.5f; // Размер иконок кнопок
    float spacing = 10.0f; // Отступ между кнопками
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.161f, 0.169f, 0.184f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.161f, 0.169f, 0.184f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.161f, 0.169f, 0.184f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::SetNextItemWidth(x * 0.4);
    if (ImGui::InputTextMultiline("##Input", buf, sizeof(buf), ImVec2(x*0.4, y*0.042), ImGuiInputTextFlags_CtrlEnterForNewLine | ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (strlen(buf) > 0) {
            Message newMsg;
            newMsg.text = std::string(buf);
            newMsg.sender = u.name;
            newMsg.time = M.getTime();
            newMsg.isText = true;
            newMsg.isImage = false;
            newMsg.doc = false;
            newMsg.avaTexture = u.avaTexture;
            newMsg.uid = u.uid;
            msgs.push_back(newMsg);
            i.sendMsg(0, newMsg);
            buf[0] = '\0';
        }
    }
    ImGui::PopStyleColor(4);

    ImGui::SameLine();
    if (ImGui::Button(ICON_MD_FILE_UPLOAD, ImVec2(iconSize, iconSize))) {
        OPENFILENAMEW ofn;
        wchar_t szFile[MAX_PATH] = { 0 };
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = L"All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameW(&ofn)) {
            Message m;
            m.sender = u.name;
            m.time = M.getTime();
            m.doc = true;
            m.id = M.getIdMsg();
            m.uid = u.uid;
            i.sendMsg(1, m);
            m.avaTexture = u.avaTexture;
            msgs.push_back(std::move(m));
        }
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(U8("Загрузить файл"));
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button(ICON_MD_IMAGE_SEARCH, ImVec2(iconSize, iconSize))) {
        OPENFILENAMEW ofn;
        wchar_t szFile[MAX_PATH] = { 0 };
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = L"Images\0*.jpg;*.jpeg;*.png;*.bmp\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameW(&ofn)) {
            Message m;
            m.sender = u.name;
            m.time = M.getTime();
            m.isImage = true;
            m.id = M.getIdMsg();
            m.uid = u.uid;
            m.mediaTexture = (ImTextureID)dx.loadImage(g_pd3dDevice, ofn.lpstrFile);
            dx.textureToBase64(g_pd3dDevice, (ID3D10ShaderResourceView*)m.mediaTexture, m.media64, m.w, m.h);
            i.sendMsg(1, m);
            m.avaTexture = u.avaTexture;
            msgs.push_back(std::move(m));
        }
    }
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(U8("Загрузить изображение"));
    ImGui::PopStyleColor();
    ImGui::EndChild();
    ImGui::End();
}


void UI::console(HWND hwnd, float x, float y) {
}
char login[32];
char ps[1000];
bool showPassword;
bool sogl;
bool showTermsWindow;

void UI::settingsw(HWND hwnd, float x, float y) {
    ImGui::Begin("settingsw", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowSize(ImVec2(x * 0.4, y * 0.3));
    ImGui::SetWindowPos(ImVec2(0, y*0.7));

    ImGui::Text(U8("тут настройки, потом сделаю"));

    ImGui::End();
}

void UI::sogll() {
    if (showTermsWindow) {
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin(U8("Пользовательское соглашение"), &showTermsWindow,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        ImGui::Text(U8("УСЛОВИЯ ИСПОЛЬЗОВАНИЯ СЕРВИСА"));
        ImGui::Separator();

        ImGui::BeginChild("TermsText", ImVec2(0, 300), true);
        ImGui::TextWrapped(U8(R"(
УСЛОВИЯ ИСПОЛЬЗОВАНИЯ СЕРВИСА ZIPCORD

1. КОНФИДЕНЦИАЛЬНОСТЬ И НЕРАСПРОСТРАНЕНИЕ ИНФОРМАЦИИ
  1.1. Пользователь (физическое лицо, использующее Zipcord и зарегистрированное в соответствии с установленным порядком) обязуется НЕ распространять информацию о сервисе Zipcord (далее - Сервис), сам Сервис, материалы из него, а также НЕ упоминать его каким-либо образом среди лиц, не являющихся пользователями Сервиса.
  1.2. Разрешается делиться информацией о Сервисе исключительно с другими зарегистрированными пользователями Zipcord.
  1.3. В случае несоблюдения данных требований учетная запись пользователя будет заблокирована.

2. ЗАЩИТА ДАННЫХ ПОЛЬЗОВАТЕЛЕЙ
  2.1. Разработчики и иные лица, задействованные в разработке или поддержке работоспособности Сервиса, НЕ передают данные и материалы пользователей третьим лицам.
  2.2. Сервис не собирает персональную информацию о пользователях, за исключением IP-адреса для обеспечения безопасности подключения.
  2.3. В случае выявления утечки любой информации о Сервисе или из него, ответственное лицо будет немедленно лишено всех полномочий и полностью заблокировано.

3. ДОСТУП К ИСХОДНОМУ КОДУ
  3.1. Исходный код Сервиса (без некоторых элементов безопасности) раскрывается по запросу в техническую поддержку.
  3.2. Пользователь, получивший исходный код, обязуется не распространять его, сведения о нем, принципы работы и иную информацию о нем лицам, не являющимся пользователями Сервиса.
  3.3. В случае нарушения данных правил будет произведена приостановка работы Сервиса, а также блокировка аккаунтов всех лиц, нарушивших условия.
)"));
        ImGui::TextWrapped(U8(R"(
4. ОБНАРУЖЕНИЕ УЯЗВИМОСТЕЙ И БЕЗОПАСНОСТЬ
4.1.При обнаружении уязвимости, бага или иной недоработки в работе Сервиса пользователь обязан немедленно сообщить в техническую поддержку, предоставив подробные сведения о проблеме.
4.2.Запрещается использование обнаруженных недоработок для извлечения личной выгоды, атак на инфраструктуру Сервиса или нанесения ущерба другим пользователям.
4.3.В случае умышленного умалчивания об уязвимости или ее эксплуатации будет произведена приостановка работы Сервиса и блокировка аккаунта нарушителя.

5. ПРАВА И ОБЯЗАННОСТИ ПОЛЬЗОВАТЕЛЕЙ
5.1.Все пользователи Сервиса, включая разработчиков, обладают равными правами.
5.2.Исключительным правом разработчиков является доступ к технической поддержке для ответов на обращения пользователей.
5.3.Все решения о добавлении новых пользователей или исключении существующих принимаются путем голосования.
5.4.Решение считается принятым только при единогласном одобрении(100 % голосов) всех участвующих в голосовании пользователей.

6. ОБЩИЕ ПОЛОЖЕНИЯ
6.1.Регистрируясь в Сервисе, пользователь подтверждает, что ознакомлен с настоящими условиями и принимает их в полном объеме.
6.2.Администрация Сервиса оставляет за собой право вносить изменения в настоящее соглашение с уведомлением пользователей.
6.3.Использование Сервиса после внесения изменений означает согласие с обновленными условиями.

Дата вступления в силу : [текущая дата]
)"));


        ImGui::EndChild();
        ImGui::End();

    }
}
void UI::registr(HWND hwnd, float x, float y) {
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);

    // Стиль окна
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 30));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

    ImGui::Begin(U8("Регистрация"), nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    // Заголовок
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(U8("Создать учетную запись")).x) * 0.5f);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), U8("Создать учетную запись"));
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Поле для логина
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), U8("Логин"));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.184f, 0.192f, 0.212f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));

    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##login", login, IM_ARRAYSIZE(login));
    ImGui::PopStyleColor(3);

    ImGui::Spacing();

    // Поле для пароля
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), U8("Пароль"));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.184f, 0.192f, 0.212f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.95f, 0.95f, 0.95f, 1.0f));

    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##password", ps, IM_ARRAYSIZE(ps),
        showPassword ? 0 : ImGuiInputTextFlags_Password);
    ImGui::PopStyleColor(3);

    // Чекбокс для показа пароля
    ImGui::Spacing();
    ImGui::Checkbox(U8("Показать пароль"), &showPassword);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Выбор аватарки
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), U8("Аватарка"));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.184f, 0.192f, 0.212f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.55f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.4f, 0.75f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    if (ImGui::Button(U8("Выбрать файл..."), ImVec2(-1, 35))) {
        
    }
    ImGui::PopStyleColor(4);

    // Отображение выбранного файла
    /*if (!g_UserData.avatarPath.empty())
    {
        ImGui::TextDisabled("Выбрано: %s", g_UserData.avatarPath.c_str());
    }*/

    // Превью аватарки
    /*if (g_UserData.avatarTexture)
    {
        ImGui::Spacing();
        ImGui::Text("Предпросмотр:");
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 80) * 0.5f);
        ImGui::Image(g_UserData.avatarTexture, ImVec2(80, 80));
    }*/

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Чекбокс соглашения
    ImGui::BeginGroup();
    ImGui::Checkbox("##agree", &sogl);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), U8("Я принимаю соглашение"));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    if (ImGui::Button(U8("Смотреть соглашение"))){
        showTermsWindow = true;
    }

    ImGui::PopStyleColor(4);
    ImGui::EndGroup();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Кнопка регистрации
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.184f, 0.192f, 0.212f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.55f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.4f, 0.75f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    if (ImGui::Button(U8("Зарегистрироваться"), ImVec2(-1, 45)))
    {
        if (strlen(login) == 0 || strlen(ps) == 0)
        {
            MessageBoxA(NULL, "Заполните все обязательные поля", "Ошибка", MB_ICONERROR);
        }
        else if (strlen(ps) < 8)
        {
            MessageBoxA(NULL, "Пароль должен содержать не менее 8 символов", "Ошибка", MB_ICONERROR);
        }
        else if (!sogl)
        {
            MessageBoxA(NULL, "Необходимо принять условия использования", "Ошибка", MB_ICONERROR);
        }
        else
        {
            u.regged = true;
            shouldRender = true;
            MessageBoxA(NULL, "Регистрация успешно завершена!", "Успех", MB_ICONINFORMATION);
            
        }
    }
    ImGui::PopStyleColor(4);

    // Подсказки
    ImGui::Spacing();
    ImGui::TextDisabled(U8(" Логин должен быть уникальным"));
    ImGui::TextDisabled(U8(" Пароль: минимум 8 символов"));
    ImGui::TextDisabled(U8(" Поддерживаемые форматы: JPG, PNG, BMP"));

    ImGui::End();
    ImGui::PopStyleVar(3); // Восстанавливаем стили
}
void UI::auth(HWND hwnd, float x, float y) {

}
void UI::acc(HWND hwnd, float x, float y) {

}
void UI::react(HWND hwnd, float x, float y) {

}
void UI::online(HWND hwnd, float x, float y) {

}

std::map<std::string, ImVec4> th_da = { {"btn", ImVec4(0.322f, 0.322f, 0.322f, 1.0f)}, {"window", ImVec4(0.173f, 0.184f, 0.2f, 1.0f)}, {"title", ImVec4(0.137f, 0.153f, 0.165f, 1.0f)}, {"border", ImVec4(0.137f, 0.153f, 0.165f, 1.0f)}, {"inputtext", ImVec4(0.322f, 0.322f, 0.322f, 1.0f)} }; // fix
std::map<std::string, ImVec4> th_de = { {"btn", ImVec4(0.173f, 0.184f, 0.2f, 1.0f)}, {"window", ImVec4(0.173f, 0.184f, 0.2f, 1.0f)}, {"title", ImVec4(0.137f, 0.153f, 0.165f, 1.0f)}, {"border", ImVec4(0.137f, 0.153f, 0.165f, 1.0f)}, {"inputtext", ImVec4(0.322, 0.322, 0.322, 1.0)} };
std::map<std::string, ImVec4> th_li = { {"btn", ImVec4(0.941f, 0.941f, 0.941f, 1.0f)}, {"window", ImVec4(1, 1, 1, 1.0f)}, {"title", ImVec4(1, 1, 1, 1.0f)}, {"border", ImVec4(0.984, 0.973, 1, 1.0f)}, {"inputtext", ImVec4(0.82, 0.82, 0.82, 1.0)}, {"text", ImVec4(0.0, 0.0, 0.0, 1.0f)} };
std::map<std::string, ImVec4> th_x_ = { {"btn", ImVec4(0.302f, 0.302f, 0.302f, 1.0f)}, {"window", ImVec4(0.0, 0.0, 0.0, 1.0f)}, {"title", ImVec4(0.271, 0.271, 0.271, 1.0f)}, {"border", ImVec4(1.0, 1.0, 1.0, 1.0f)}, {"inputtext", ImVec4(0.137, 0.161, 0.188, 1.0)}, {"text", ImVec4(0.0, 0.0, 0.0, 1.0f)}, {"btn_li", ImVec4(0.878, 0.878, 0.878, 1.0)} };
std::map<std::string, ImVec4> th_rb = { {"btn", ImVec4(0.929f, 0.0f, 1.0f, 1.0f)}, {"window", ImVec4(1, 0.325, 0.918, 1.0f)}, {"title", ImVec4(0.984, 0.757, 1, 1.0f)}, {"border", ImVec4(1, 0, 0.878, 1.0f)}, {"inputtext", ImVec4(0.788, 0.024, 0.718, 1.0)} };
std::map<std::string, ImVec4> th_pu = { {"btn", ImVec4(0.518f, 0.271f, 0.91f, 1.0f)}, {"window", ImVec4(0.455, 0, 0.62, 1.0f)}, {"title", ImVec4(0.827, 0.518, 0.941, 1.0f)}, {"border", ImVec4(0.392, 0.18, 0.471, 1.0f)}, {"inputtext", ImVec4(0.482, 0.255, 0.678, 1.0)} };


void UI::set_theme(std::string tm) {
    ImGuiStyle& style = ImGui::GetStyle();
    if (tm == "default") {
        style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] = th_de["window"];
        style.Colors[ImGuiCol_Button] = th_de["btn"];
        style.Colors[ImGuiCol_FrameBg] = th_de["inputtext"];
        style.Colors[ImGuiCol_TitleBgActive] = th_de["title"];
        style.Colors[ImGuiCol_ButtonHovered] = th_x_["btn"];
        style.Colors[ImGuiCol_ButtonActive] = th_x_["btn_li"];
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_ChildBg] = th_de["inputtext"];
        std::cout << "Theme set to default" << std::endl;
    }
    else if (tm == "x") {
        style.Colors[ImGuiCol_Border] = th_x_["border"];
        style.Colors[ImGuiCol_WindowBg] = th_x_["window"];
        style.Colors[ImGuiCol_FrameBg] = th_x_["inputtext"];
        style.Colors[ImGuiCol_TitleBgActive] = th_x_["title"];
        style.Colors[ImGuiCol_Button] = th_x_["btn"];
        style.Colors[ImGuiCol_ButtonHovered] = th_x_["btn"];
        style.Colors[ImGuiCol_ButtonActive] = th_x_["btn_li"];
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        std::cout << "Theme set to x" << std::endl;
    }
    else if (tm == "rb") {
        style.Colors[ImGuiCol_Border] = th_rb["border"];
        style.Colors[ImGuiCol_WindowBg] = th_rb["window"];
        style.Colors[ImGuiCol_FrameBg] = th_rb["inputtext"];
        style.Colors[ImGuiCol_TitleBgActive] = th_rb["title"];
        style.Colors[ImGuiCol_Button] = th_rb["btn"];

        std::cout << "Theme set to rb" << std::endl;
    }
    else if (tm == "light") {
        style.Colors[ImGuiCol_Border] = th_li["border"];
        style.Colors[ImGuiCol_WindowBg] = th_li["window"];
        style.Colors[ImGuiCol_FrameBg] = th_li["inputtext"];
        style.Colors[ImGuiCol_TitleBgActive] = th_li["title"];
        style.Colors[ImGuiCol_Button] = th_li["btn"];
        style.Colors[ImGuiCol_Text] = th_li["text"];
        std::cout << "Theme set to light" << std::endl;
    }
    else if (tm == "dark") {
        style.Colors[ImGuiCol_Border] = th_da["border"];
        style.Colors[ImGuiCol_WindowBg] = th_da["window"];
        style.Colors[ImGuiCol_FrameBg] = th_da["inputtext"];
        style.Colors[ImGuiCol_TitleBgActive] = th_da["title"];
        style.Colors[ImGuiCol_Button] = th_da["btn"];
        std::cout << "Theme set to dark" << std::endl;
    }
    else if (tm == "purple") {
        style.Colors[ImGuiCol_Border] = th_pu["border"];
        style.Colors[ImGuiCol_WindowBg] = th_pu["window"];
        style.Colors[ImGuiCol_FrameBg] = th_pu["inputtext"];
        style.Colors[ImGuiCol_TitleBgActive] = th_pu["title"];
        style.Colors[ImGuiCol_Button] = th_pu["btn"];
        std::cout << "Theme set to purple" << std::endl;
    }
    shouldRender = true;
}
