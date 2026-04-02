#include "gui.h"
#include <imgui.h>
#include "language.h"
#include <string>
#include "serverconfig.h"
#include <Windows.h>
#include "ModConfig.h"

void gui_setting_menu(ImVec2 center);
void gui_about_menu(ImVec2 center);
void gui_helpme_menu(ImVec2 center);
void r_SetToolTip(const char* Text);

void DetectLanguageBySystemLanguage();

namespace gui{
    static float slider_value = 0.5f;
    static int counter = 0;
    static bool open_helpme_popup = false;
    static bool open_about_popup = false;
    static bool open_setting_popup = false;

    void init(){
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;

        ImFontConfig config;
        config.OversampleH = 1;
        config.OversampleV = 1;
        config.PixelSnapH = true;

        io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc",18.0f,&config,io.Fonts->GetGlyphRangesChineseFull());

        DetectLanguageBySystemLanguage();
    }

    void render() {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_MenuBar;

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
        if (open_helpme_popup) {
            ImGui::OpenPopup(t(u8"帮我窗口"));
            open_helpme_popup = false;
        }
        if (open_setting_popup) {
            ImGui::OpenPopup(t(u8"设置窗口"));
            open_setting_popup = false;
        }
        if (open_about_popup) {
            ImGui::OpenPopup(t(u8"关于窗口"));
            open_about_popup = false;
        }

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 center = viewport->GetCenter();

        gui_setting_menu(center);
        gui_about_menu(center);
        gui_helpme_menu(center);

        if (ImGui::BeginTabBar("MainTabs"))
        {
            if (ImGui::BeginTabItem(t(u8"基础配置")))
            {
                if (ImGui::BeginTable("base_config_table",2,ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV))
                {
                    ImGui::TableSetupColumn(t(u8"标签"), ImGuiTableColumnFlags_WidthFixed, 120.0f);
                    ImGui::TableSetupColumn(t(u8"控件"), ImGuiTableColumnFlags_WidthStretch);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"名称"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##名称", sc_Name, sizeof(sc_Name));
                    r_SetToolTip(t(u8"服务器名称 允许使用非英文文本"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"密码"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##密码", sc_Password, sizeof(sc_Password));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"地图"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##地图", sc_Map, sizeof(sc_Map));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"最大玩家数量"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##最大玩家数量", sc_Maxplayer, sizeof(sc_Maxplayer));
                    r_SetToolTip(t(u8"服务器允许的最大玩家数量上限"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"游戏难度"));
                    ImGui::TableSetColumnIndex(1);
                    std::string difficulty_items = std::string(t(u8"简单")) + '\0' + t(u8"普通") + '\0' + t(u8"困难") + '\0';

                    ImGui::Combo("##游戏难度",&sc_Mode_item,difficulty_items.c_str());

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"允许PVP"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Checkbox("##允许PVP", &sc_PVP);
                    r_SetToolTip(t(u8"是否允许玩家互相攻击"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"端口"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##端口", sc_Port, sizeof(sc_Port));
                    r_SetToolTip(t(u8"服务器端口 只能是五位数!"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"作弊"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Checkbox("##作弊", &sc_Cheats);
                    r_SetToolTip(t(u8"是否允许使用/day /give等指令"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"视角"));
                    ImGui::TableSetColumnIndex(1);

                    std::string perspective_items = std::string(t(u8"两者")) + '\0' + t(u8"第一人称") + '\0' + t(u8"第三人称") + '\0';

                    ImGui::Combo("##视角",&sc_Perspective_item,perspective_items.c_str());

                    r_SetToolTip(t(u8"是否允许切换视角或固定视角"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"初始物资"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##初始物资", sc_Loadout, sizeof(sc_Loadout));
                    r_SetToolTip(t(u8"初始物资 只允许输入Id 使用\"/\"分隔"));

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(t(u8"欢迎语"));
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##欢迎语", sc_Welcome, sizeof(sc_Welcome));

                    ImGui::Separator();

                    if (ImGui::Button(t(u8"保存配置")))
                        SaveServerConfig();

                    ImGui::EndTable();
                }

                ImGui::EndTabItem();

            }

            if (ImGui::BeginTabItem(t(u8"生成配置")))
            {
                ImGui::Text(t(u8"这个配置太复杂了呢~ 请耐心等待更新哦 喵~"));
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem(t(u8"模组配置")))
            {
                static char newModID[32] = {};

                if (ImGui::Button(t(u8"加载模组 ID")))
                    LoadFileIDs();
                ImGui::SameLine();
                if (ImGui::Button(t(u8"保存模组 ID")))
                    SaveFileIDs();

                ImGui::Separator();

                if (ImGui::BeginTable("mod_table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchSame))
                {
                    ImGui::TableSetupColumn(u8"模组 ID");
                    ImGui::TableSetupColumn(u8"操作", ImGuiTableColumnFlags_WidthFixed, 80.0f);

                    for (size_t i = 0; i < FileIDs.size(); ++i)
                    {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", FileIDs[i].c_str());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::PushID(static_cast<int>(i));
                        if (ImGui::Button(t(u8"删除")))
                        {
                            RemoveFileID(i);
                            ImGui::PopID();
                            break;
                        }
                        ImGui::PopID();
                    }

                    ImGui::EndTable();
                }

                ImGui::Separator();

                ImGui::InputText(t(u8"新模组 ID"), newModID, sizeof(newModID));
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

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}

void gui_helpme_menu(ImVec2 center) {

    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(t(u8"帮我窗口"), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::Text(t(u8"如何获取地图名称?\n  在D:\\steam\\steamapps\\common\\Unturned\\Maps\n内的文件夹就是地图名称\n  创意工坊地图在D:\\steam\\steamapps\\workshop\\content\\304930\n内找到对应ID的文件夹打开后看到的文件夹就是地图名称"));
        ImGui::Separator();

        if (ImGui::Button(t(u8"确定"), ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void gui_setting_menu(ImVec2 center) {
	
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(t(u8"设置窗口"), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
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

void gui_about_menu(ImVec2 center) {
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(t(u8"关于窗口"), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::Text(t(u8"开发者 Aria1337"));
        ImGui::Text(t(u8"为我的朋友\"无聊白宁\"制作"));

        ImGui::Separator();

        if (ImGui::Button(t(u8"确定"), ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void r_SetToolTip(const char* Text) {
	ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.3f, 1.0f), u8"(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(Text);
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