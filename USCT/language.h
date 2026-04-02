#pragma once
#include <string>
#include <unordered_map>
#include "setting.h"

namespace lang
{
    using dict_t = std::unordered_map<std::string, std::string>;

    inline dict_t zh =
    {
        { u8"文件", u8"文件" },
        { u8"帮助", u8"帮助" },
        { u8"设置", u8"设置" },
        { u8"打开", u8"打开" },
        { u8"保存", u8"保存" },

        { u8"关于", u8"关于" },
        { u8"设置窗口", u8"设置窗口" },
        { u8"确定", u8"确定" },
        { u8"基础配置", u8"基础配置" },
        { u8"生成配置", u8"生成配置" },
        { u8"名称", u8"名称" },
        { u8"密码", u8"密码" },
        { u8"地图", u8"地图" },
        { u8"最大玩家数量", u8"最大玩家数量" },
        { u8"游戏难度", u8"游戏难度" },
        { u8"允许PVP", u8"允许PVP" },
        { u8"端口", u8"端口" },
        { u8"作弊", u8"作弊" },
        { u8"视角", u8"视角" },
        { u8"初始物资", u8"初始物资" },
        { u8"欢迎语", u8"欢迎语" },

        { u8"服务器名称 允许使用非英文文本", u8"服务器名称 允许使用非英文文本" },
        { u8"服务器允许的最大玩家数量上限", u8"服务器允许的最大玩家数量上限" },
        { u8"是否允许玩家互相攻击", u8"是否允许玩家互相攻击" },
        { u8"服务器端口 只能是五位数!", u8"服务器端口 只能是五位数!" },
        { u8"是否允许使用/day /give等指令", u8"是否允许使用/day /give等指令" },
        { u8"是否允许切换视角或固定视角", u8"是否允许切换视角或固定视角" },
        { u8"初始物资 只允许输入Id 使用\"/\"分隔", u8"初始物资 只允许输入Id 使用\"/\"分隔" }
        // 实际上是可以不写的 会直接返回参数 但不写不好看 懂我意思吗?
    };

    inline dict_t en =
    {
        { u8"文件", u8"File" },
        { u8"帮助", u8"Help" },
        { u8"设置", u8"Settings" },
        { u8"打开", u8"Open" },
        { u8"保存", u8"Save" },
        { u8"帮我", u8"HelpMe" },
        
        { u8"关于", u8"About" },
        { u8"关于窗口", u8"About Window" },
        { u8"设置窗口", u8"Settings Window" },
        { u8"帮我窗口", u8"HelpMe Window" },
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

        { u8"模组配置", u8"ModConfig" },
        { u8"保存配置", u8"Save Config" },
        { u8"加载模组 ID", u8"LoadModID" },
        { u8"保存模组 ID", u8"SaveModID" },
        { u8"删除", u8"Delete" },
        { u8"新模组 ID", u8"New ModID" },
        { u8"添加", u8"Add" },

        { u8"简单", u8"Easy" },
        { u8"普通", u8"Normal" },
        { u8"困难", u8"Hard" },
        { u8"两者", u8"both" },
        { u8"第一人称", u8"first" },
        { u8"第三人称", u8"third" },

        { u8"开发者 Aria1337", u8"Developer Aria1337" },
        { u8"为我的朋友\"无聊白宁\"制作", u8"Made for my friend “Bored BaiNing”" },
        { u8"这个配置太复杂了呢~ 请耐心等待更新哦 喵~", u8"This config is a bit too complicated~ Please be patient updates, nya~" },

        { u8"服务器名称 允许使用非英文文本", u8"Server name (non-English text allowed)" },
        { u8"服务器允许的最大玩家数量上限", u8"Maximum number of players allowed on the server" },
        { u8"是否允许玩家互相攻击", u8"Whether players are allowed to attack each other" },
        { u8"服务器端口 只能是五位数!", u8"Server port must be a five-digit number!" },
        { u8"是否允许使用/day /give等指令", u8"Whether commands like /day and /give are allowed" },
        { u8"是否允许切换视角或固定视角", u8"Whether switching view modes or fixed view is allowed" },
        { u8"初始物资 只允许输入Id 使用\"/\"分隔", u8"Starting items: only item IDs allowed, separated by \"/\"" },
        { u8"如何获取地图名称?\n  在D:\\steam\\steamapps\\common\\Unturned\\Maps\n内的文件夹就是地图名称\n  创意工坊地图在D:\\steam\\steamapps\\workshop\\content\\304930\n内找到对应ID的文件夹打开后看到的文件夹就是地图名称", u8"How do I find the map name?\n  The folder located at D:\\steam\\steamapps\\common\\Unturned\\Maps\n is the map name.\n  For Workshop maps, go to \nD:\\steam\\steamapps\\workshop\\content\\304930\n and open the folder corresponding to the map ID\nthe folder you see inside is the map name." }
    };

    inline const char* get(
        const std::string& key
    )
    {
        const dict_t* table = nullptr;

        if (setting::language == chinese)
        {
            table = &zh;
        }
        else
        {
            table = &en;
        }

        auto it = table->find(key);

        if (it != table->end())
            return it->second.c_str();

        return key.c_str();
    }
}

#define t(key) lang::get(key)