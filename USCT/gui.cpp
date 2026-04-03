#include "gui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "language.h"
#include <string>
#include "serverconfig.h"
#include <Windows.h>
#include "ModConfig.h"

#define tip(A,B,C) ImGui::SameLine();ImGui::TextColored(A, B);if (ImGui::IsItemHovered()){ImGui::SetTooltip(C);}

void gui_setting_menu(ImVec2 center);
void gui_TextMenu_menu(ImVec2 center, const char name[16], const char text[256]);

void DetectLanguageBySystemLanguage();

namespace gui{
    static float slider_value = 0.5f;
    static int counter = 0;
    static bool open_helpme_popup = false;
    static bool open_about_popup = false;
    static bool open_setting_popup = false;

    void init(){
        ImFontConfig config;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiContext& g = *ImGui::GetCurrentContext();
        ImGuiStyle& style = ImGui::GetStyle();
        io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesChineseFull());

        config.OversampleH = 1;
        config.OversampleV = 1;
        config.PixelSnapH = true;

        g.ConfigNavWindowingKeyNext = 0;
        g.ConfigNavWindowingKeyPrev = 0;

        style.FrameRounding = 5.0f;
        style.PopupRounding = 5.0f;
        style.ScrollbarRounding = 5.0f;
        style.GrabRounding = 5.0f;
        style.TabRounding = 5.0f;
        style.ChildRounding = 5.0f;
        style.PopupRounding = 5.0f;
        style.SeparatorTextBorderSize = 1.7f;

        DetectLanguageBySystemLanguage();
    }

    void render() {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

        ImGui::Begin("MainWindow", nullptr, flags);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu(t(u8"文件"))) {
                if (ImGui::MenuItem(t(u8"保存")))
                    SaveServerConfig();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(t(u8"帮助"))) {
                if (ImGui::MenuItem(t(u8"帮我"))) {
                    open_helpme_popup = true;
                }
                if (ImGui::MenuItem(t(u8"关于"))) {
                    open_about_popup = true;
                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem(t(u8"设置")))
                open_setting_popup = true;

            ImGui::EndMenuBar();
        }
        if (open_setting_popup) {
            ImGui::OpenPopup(t(u8"设置"));
            open_setting_popup = false;
        }
        if (open_about_popup) {
            ImGui::OpenPopup(t(u8"关于"));
            open_about_popup = false;
        }
        if (open_helpme_popup) {
            ImGui::OpenPopup(t(u8"帮我"));
            open_helpme_popup = false;
        }

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 center = viewport->GetCenter();

        gui_setting_menu(center);
        gui_TextMenu_menu(center, u8"关于",u8"开发者 Aria1337\n为我的朋友\"无聊白宁\"制作");
        gui_TextMenu_menu(center, u8"帮我", u8"如何获取地图名称?\n  在D:\\steam\\steamapps\\common\\Unturned\\Maps\n内的文件夹就是地图名称\n  创意工坊地图在D:\\steam\\steamapps\\workshop\\content\\304930\n内找到对应ID的文件夹打开后看到的文件夹就是地图名称");

        if (ImGui::BeginTabBar("MainTabs"))
        {
            if (ImGui::BeginTabItem(t(u8"基础配置")))
            {
                if (ImGui::Button(t(u8"加载配置"), ImVec2(275, 0)))
                    LoadServerConfig();
                ImGui::SameLine();
                if (ImGui::Button(t(u8"保存配置"),ImVec2(275,0)))
                    SaveServerConfig();
                ImGui::BeginChild("Child1", ImVec2(0, 0), false);
                if (ImGui::BeginTable("base_config_table",2,ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV))
                {
                    ImGui::TableSetupColumn(t(u8"标签"), ImGuiTableColumnFlags_WidthFixed, 150.0f);
                    ImGui::TableSetupColumn(t(u8"控件"), ImGuiTableColumnFlags_WidthStretch);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"名称"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##名称", sc_Name, sizeof(sc_Name));
                    tip(ImVec4(0.3f,0.3f,0.3f,1.0f),"(?)", t(u8"服务器名称 允许使用非英文文本"))
                    
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"端口"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##端口", sc_Port, sizeof(sc_Port));
                    tip(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)", t(u8"服务器端口 只能是五位数!"))

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"最大玩家数量"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##最大玩家数量", sc_Maxplayer, sizeof(sc_Maxplayer));
                    tip(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)", t(u8"服务器允许的最大玩家数量上限"))

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"地图"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##地图", sc_Map, sizeof(sc_Map));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"游戏难度"));
                    ImGui::TableSetColumnIndex(1);
                    std::string difficulty_items = std::string(t(u8"简单")) + '\0' + t(u8"普通") + '\0' + t(u8"困难") + '\0';
                    ImGui::Combo("##游戏难度", &sc_Mode_item, difficulty_items.c_str());

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"视角"));
                    ImGui::TableSetColumnIndex(1);
                    std::string perspective_items = std::string(t(u8"两者")) + '\0' + t(u8"第一人称") + '\0' + t(u8"第三人称") + '\0';
                    ImGui::Combo("##视角", &sc_Perspective_item, perspective_items.c_str());
                    tip(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)", t(u8"是否允许切换视角或固定视角"))

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"允许PVP"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Checkbox("##允许PVP", &sc_PVP);
                    tip(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)", t(u8"是否允许玩家互相攻击"))

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"作弊"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Checkbox("##作弊", &sc_Cheats);
                    tip(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)", t(u8"是否允许使用/day /give等指令"))

                    ImGui::EndTable();
                }
                ImGui::SeparatorText(t(u8"可选项"));
                if (ImGui::BeginTable("base_config_table2", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV))
                {
                    ImGui::TableSetupColumn(t(u8"标签"), ImGuiTableColumnFlags_WidthFixed, 150.0f);
                    ImGui::TableSetupColumn(t(u8"控件"), ImGuiTableColumnFlags_WidthStretch);

                        ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"密码"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##密码", sc_Password, sizeof(sc_Password));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"初始物资"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##初始物资", sc_Loadout, sizeof(sc_Loadout));
                    tip(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)", t(u8"初始物资 只允许输入Id 使用\"/\"分隔"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"服务器所有者"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##服务器所有者", sc_Owner, sizeof(sc_Owner));
                    tip(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)", t(u8"服务器的所有者 只允许输入数字SteamID 只允许一个有所有者"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"GSLT"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##GSLT", sc_GSLT, sizeof(sc_GSLT));
                    tip(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), "(?)", t(u8"游戏服务器登录令牌 建立公开游戏服务器的必要条件\n根据\"内网穿透工具\"的不同可以选择不填"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"欢迎语"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##欢迎语", sc_Welcome, sizeof(sc_Welcome));

                    ImGui::EndTable();
                }ImGui::EndChild();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(t(u8"生成配置")))
            {
                ImGui::Text(t(u8"这个配置太复杂了呢~ 请耐心等待更新哦 喵~"));
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(t(u8"模组配置")))
            {
                static char newModID[13] = {};

                if (ImGui::Button(t(u8"加载模组"), ImVec2(275, 0)))
                    LoadFileIDs();
                ImGui::SameLine();
                if (ImGui::Button(t(u8"保存模组"), ImVec2(275, 0)))
                    SaveFileIDs();

                ImGui::Separator();
                if (ImGui::BeginChild("Childl", ImVec2(220, 250), true)) {
                    if (ImGui::BeginTable("mod_table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchSame))
                    {
                        ImGui::TableSetupColumn(u8"模组ID", ImGuiTableColumnFlags_WidthFixed, 130.0f);
                        ImGui::TableSetupColumn(u8"操作");

                        for (size_t i = 0; i < FileIDs.size(); ++i)
                        {
                            ImGui::TableNextRow();

                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%s", FileIDs[i].c_str());

                            ImGui::TableSetColumnIndex(1);
                            ImGui::PushID(static_cast<int>(i));
                            if (ImGui::Button(t(u8"删除"),ImVec2(60,0)))
                            {
                                RemoveFileID(i);
                                ImGui::PopID();
                                break;
                            }
                            ImGui::PopID();
                        }

                        ImGui::EndTable();
                    }
                }ImGui::EndChild();
                ImGui::SameLine();
                if (ImGui::BeginChild("Childr", ImVec2(0, 0), false)) {
                    ImGui::SetNextItemWidth(150);
                    ImGui::InputText(t(u8"模组ID"), newModID, sizeof(newModID));
                    tip(ImVec4(0.6f, 0.4f, 0.4f, 1.0f), "( ! )", t(u8"不是模组名称"));
                    ImGui::SameLine();
                    if (ImGui::Button(t(u8"添加")))
                    {
                        std::string idStr(newModID);
                        if (!idStr.empty())
                        {
                            AddFileID(idStr);
                            newModID[0] = '\0';
                        }
                    }
                }ImGui::EndChild();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}

void gui_setting_menu(ImVec2 center) {
	
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(t(u8"设置"), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        static int language_item = setting::language - 1;

        ImGui::Combo(u8"Language", &language_item, u8"中文\0English\0");
        setting::language = language_item + 1;

        ImGui::Separator();

        if (ImGui::Button(t(u8"确定"), ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void gui_TextMenu_menu(ImVec2 center,const char name[16], const char text[256]) {
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(t(name), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::Text(t(text));

        ImGui::Separator();

        if (ImGui::Button(t(u8"确定"), ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void DetectLanguageBySystemLanguage()
{
    LANGID langId = GetUserDefaultUILanguage();

    WORD primaryLang = PRIMARYLANGID(langId);

    if (primaryLang == LANG_CHINESE)
    {
        setting::language = chinese;
    }
    else
    {
        setting::language = english;
    }
}