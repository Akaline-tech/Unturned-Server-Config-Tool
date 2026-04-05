#pragma once
#include <string>
#include <unordered_map>
#include "setting.h"

namespace lang
{
    using dict_t = std::unordered_map<std::string, std::string>;

    inline dict_t en =
    {
        { u8"文件", u8"File" },
        { u8"帮助", u8"Help" },
        { u8"设置", u8"Settings" },
        { u8"打开", u8"Open" },
        { u8"保存", u8"Save" },
        { u8"帮我", u8"HelpMe" },
        { u8"错误", u8"Error" },
        { u8"可选项", u8"Optional" },
        
        { u8"关于", u8"About" },
        { u8"设置", u8"Settings" },
        { u8"帮我", u8"HelpMe" },
        { u8"确定", u8"OK" },
        { u8"基础配置", u8"Basic Configuration" },
        { u8"生成配置", u8"Generate Configuration" },
        { u8"名称", u8"Name" },
        { u8"密码", u8"Password" },
        { u8"地图", u8"Map" },
        { u8"最大玩家数量", u8"Max Players" },
        { u8"游戏难度", u8"Game Difficulty" },
        { u8"允许PVP", u8"Allow PvP" },
        { u8"端口", u8"Port" },
        { u8"作弊", u8"Cheats" },
        { u8"视角", u8"Perspective" },
        { u8"初始物资", u8"Starting Items" },
        { u8"欢迎语", u8"Welcome Message" },
        { u8"服务器所有者", u8"Server Owner" },
        { u8"GSLT", u8"GSLT" },//占位用
        { u8"无可用地图", u8"No maps available" },

        { u8"模组配置", u8"Mod Config" },
        { u8"加载配置", u8"Load Config" },
        { u8"保存配置", u8"Save Config" },
        { u8"加载模组", u8"Load Mod" },
        { u8"保存模组", u8"Save Mod" },
        { u8"删除", u8"Delete" },
        { u8"模组ID", u8"New ModID" },
        { u8"添加", u8"Add" },

        { u8"简单", u8"Easy" },
        { u8"普通", u8"Normal" },
        { u8"困难", u8"Hard" },
        { u8"两者", u8"both" },
        { u8"第一人称", u8"first" },
        { u8"第三人称", u8"third" },

        { u8"开发者 Aria1337\n为我的朋友\"无聊白宁\"制作\n以及其他贡献者(排名不分先后)\n福浮, 魔理沙, 星月, 芥酱, 洛希凌 · 莉安\nunknow084player, 可乐加冰 , 灵梦, 奈酱, 童tong, 千梨少\n", u8"Developer Aria1337\nMade for my friend “Bored BaiNing”" },
        { u8"这个配置太复杂了呢~ 请耐心等待更新哦 喵~", u8"This config is a bit too complicated~ Please be patient updates, nya~" },

        { u8"不是模组名称", u8"Not Mod Name" },
        { u8"服务器名称 允许使用非英文文本", u8"Server name (non-English text allowed)" },
        { u8"服务器允许的最大玩家数量上限", u8"Maximum number of players allowed on the server" },
        { u8"是否允许玩家互相攻击", u8"Whether players are allowed to attack each other" },
        { u8"服务器端口 允许的范围16000到45000", u8"Server port only 16000 to 45000 allowed" },
        { u8"是否允许使用/day /give等指令", u8"Whether commands like /day and /give are allowed" },
        { u8"是否允许切换视角或固定视角", u8"Whether switching view modes or fixed view is allowed" },
        { u8"初始物资 只允许输入Id 使用\"/\"分隔", u8"Starting items: only itemIDs allowed, separated by \"/\"" },
        { u8"服务器的所有者 只允许输入数字SteamID 只允许一个有所有者", u8"server owner: only numeric SteamID allowed & only one owner" }, 
        { u8"游戏服务器登录令牌 建立公开游戏服务器的必要条件\n根据\"内网穿透工具\"的不同可以选择不填", u8"Game Server Login Token : A prerequisite for setting up a public game server\nDepending on the \"NAT traversal\" method, this field may be left blank" },
        { u8"如何获取地图名称?\n  在\\steam\\steamapps\\common\\Unturned\\Maps\n内的文件夹就是地图名称\n  创意工坊地图在D:\\steam\\steamapps\\workshop\\content\\304930\n内找到对应ID的文件夹打开后看到的文件夹就是地图名称", u8"How do I find the map name?\n  The folder located at D:\\steam\\steamapps\\common\\Unturned\\Maps\n is the map name.\n  For Workshop maps, go to \nD:\\steam\\steamapps\\workshop\\content\\304930\n and open the folder corresponding to the map ID\nthe folder you see inside is the map name." }
    };

    inline const char* get(const std::string& key)
    {
        if (setting::language == chinese)
            return key.c_str();

        const dict_t* table = nullptr;

        if (setting::language == english)
        {
            table = &en;
        }
        else
        {
            table = nullptr;
        }

        if (table)
        {
            auto it = table->find(key);
            if (it != table->end())
                return it->second.c_str();
        }

        return key.c_str();
    }
}

#define t(key) lang::get(key)