#include <string>
#include <filesystem>
#include "uilt.h"

namespace fs = std::filesystem;

std::vector<std::string> GetAllFolders(const std::string& basePath)
{
    std::vector<std::string> folderNames;

    if (!fs::exists(basePath) || !fs::is_directory(basePath))
        return folderNames;

    for (const auto& entry : fs::directory_iterator(basePath))
    {
        if (entry.is_directory())
        {
            folderNames.push_back(entry.path().filename().string());
        }
    }

    return folderNames;
}

std::vector<std::string> GetMapList()
{
    std::vector<std::string> mapList;

    auto add_unique = [&](const std::string& name) {
        for (const auto& s : mapList) if (s == name) return;
        mapList.push_back(name);
    };

    std::string basePath = FindConfigPath("\\steamapps\\common\\Unturned\\Maps\\");
    if (fs::exists(basePath) && fs::is_directory(basePath))
    {
        for (const auto& entry : fs::directory_iterator(basePath))
        {
            if (entry.is_directory())
                add_unique(entry.path().filename().string());
        }
    }

    std::string workshopPath = FindConfigPath("\\steamapps\\common\\U3DS\\Servers\\Default\\Workshop\\Steam\\content\\304930");
    if (fs::exists(workshopPath) && fs::is_directory(workshopPath))
    {
        for (const auto& modEntry : fs::directory_iterator(workshopPath))
        {
            if (!modEntry.is_directory()) continue;

            fs::path modPath = modEntry.path();
            fs::path metaInMod = modPath / "Map.meta";

            if (!fs::exists(metaInMod) || fs::is_directory(metaInMod))
                continue;

            for (const auto& mapEntry : fs::directory_iterator(modPath))
            {
                if (mapEntry.is_directory())
                    add_unique(mapEntry.path().filename().string());
            }
        }
    }

    return mapList;
}

const char* GetMapItemUtf8(void* data, int idx)
{
    std::vector<std::string>& list = *(std::vector<std::string>*)data;
    if (idx < 0 || idx >= list.size()) return "";
    return (const char*)u8"" R"(%s)", list[idx].c_str();
}