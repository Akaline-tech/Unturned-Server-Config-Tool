#include "serverconfig.h"

#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

static std::string CONFIG_FILE;

static std::string FindServerConfigPath()
{
    for (char drive = 'C'; drive <= 'Z'; drive++)
    {
        std::vector<std::string> roots =
        {
            "\\steam",
            "\\Steam",
            "\\SteamLibrary"
        };

        for (const auto& root : roots)
        {
            std::string path =
                std::string(1, drive) +
                ":" +
                root +
                "\\steamapps\\common\\U3DS\\Servers\\Default\\Server\\Commands.dat";

            if (fs::exists(path))
            {
                return path;
            }
        }
    }

    // fallback 当前目录
    return "Commands.dat";
}

static void SafeCopy(char* dest, const std::string& src, size_t size)
{
    strncpy(dest, src.c_str(), size);
    dest[size - 1] = '\0';
}

static const char* GetModeString()
{
    switch (sc_Mode_item)
    {
    case 0: return "Easy";
    case 1: return "Normal";
    case 2: return "Hard";
    default: return "Easy";
    }
}

static const char* GetPerspectiveString()
{
    switch (sc_Perspective_item)
    {
    case 0: return "both";
    case 1: return "first";
    case 2: return "third";
    default: return "both";
    }
}

static void SetModeFromString(const std::string& value)
{
    if (value == "Easy")
        sc_Mode_item = 0;
    else if (value == "Normal")
        sc_Mode_item = 1;
    else if (value == "Hard")
        sc_Mode_item = 2;
}

static void SetPerspectiveFromString(const std::string& value)
{
    if (value == "both")
        sc_Perspective_item = 0;
    else if (value == "first")
        sc_Perspective_item = 1;
    else if (value == "third")
        sc_Perspective_item = 2;
}

// =====================================================
// 保存
// =====================================================

void SaveServerConfig()
{
    if (CONFIG_FILE.empty())
        CONFIG_FILE = FindServerConfigPath();

    std::ofstream file(CONFIG_FILE);

    if (!file.is_open())
        return;

    file << "Name " << sc_Name << "\n";

    file << "password "
        << sc_Password
        << "\n";

    file << "Map "
        << sc_Map
        << "\n";

    file << "Maxplayers "
        << sc_Maxplayer
        << "\n";

    file << "Mode "
        << GetModeString()
        << "\n";

    // PVP 关闭 -> 留空
    if (sc_PVP)
        file << "";
    else
        file << "PVE\n";

    file << "Port "
        << sc_Port
        << "\n";

    if(sc_Cheats)
        file << "Cheats "<< "on" << "\n";
    else
        file << "";


    file << "Perspective "
        << GetPerspectiveString()
        << "\n";

    file << "Loadout "
        << sc_Loadout
        << "\n";

    file << "Welcome "
        << sc_Welcome
        << "\n";

    file.close();
}

// =====================================================
// 加载
// =====================================================

void LoadServerConfig()
{
    if (CONFIG_FILE.empty())
        CONFIG_FILE = FindServerConfigPath();

    std::ifstream file(CONFIG_FILE);

    if (!file.is_open())
    {
        SaveServerConfig();
        return;
    }

    sc_PVP = true;
    sc_Cheats = false;

    bool foundPVE = false;

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);

        std::string key;
        std::string value;

        iss >> key;

        std::transform(
            key.begin(),
            key.end(),
            key.begin(),
            ::toupper
        );

        if (key == "NAME")
        {
            iss >> value;
            SafeCopy(sc_Name, value, sizeof(sc_Name));
        }

        else if (key == "PASSWORD")
        {
            iss >> value;
            SafeCopy(sc_Password, value, sizeof(sc_Password));
        }

        else if (key == "MAP")
        {
            iss >> value;
            SafeCopy(sc_Map, value, sizeof(sc_Map));
        }

        else if (key == "MAXPLAYERS")
        {
            iss >> value;
            SafeCopy(sc_Maxplayer, value, sizeof(sc_Maxplayer));
        }

        else if (key == "MODE")
        {
            iss >> value;
            SetModeFromString(value);
        }

        else if (key == "PVE")
        {
            sc_PVP = false;
            foundPVE = true;
        }

        else if (key == "PVP")
        {
            sc_PVP = true;
            foundPVE = true;
        }

        else if (key == "PORT")
        {
            iss >> value;
            SafeCopy(sc_Port, value, sizeof(sc_Port));
        }

        else if (key == "CHEATS")
        {
            iss >> value;

            std::transform(
                value.begin(),
                value.end(),
                value.begin(),
                ::tolower
            );

            sc_Cheats = (value == "on");
        }

        else if (key == "PERSPECTIVE")
        {
            iss >> value;
            SetPerspectiveFromString(value);
        }

        else if (key == "LOADOUT")
        {
            iss >> value;
            SafeCopy(sc_Loadout, value, sizeof(sc_Loadout));
        }

        else if (key == "WELCOME")
        {
            std::getline(iss, value);

            if (!value.empty() && value[0] == ' ')
                value.erase(0, 1);

            SafeCopy(sc_Welcome, value, sizeof(sc_Welcome));
        }
    }

    file.close();

    if (!foundPVE)
        sc_PVP = true;
}