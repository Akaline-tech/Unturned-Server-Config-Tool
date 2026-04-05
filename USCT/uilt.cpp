#include <filesystem>
#include <string>
#include <vector>
#pragma comment(lib, "winhttp.lib")
#include "uilt.h"

namespace fs = std::filesystem;
std::string FindConfigPath(std::string filepath)
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
                std::string(1, drive) + ":" + root + filepath;

            if (fs::exists(path))
            {
                return path;
            }
        }
    }

    return filepath;
}