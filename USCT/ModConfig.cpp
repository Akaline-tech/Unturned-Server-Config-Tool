#include "ModConfig.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

std::vector<std::string> FileIDs;
static std::string CONFIG_FILE;

static std::string FindWorkshopConfigPath()
{
    for (char drive = 'C'; drive <= 'Z'; ++drive)
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
                "\\steamapps\\common\\U3DS\\Servers\\Default\\WorkshopDownloadConfig.json";

            if (fs::exists(path))
                return path;
        }
    }

    // fallback 当前目录
    return "WorkshopDownloadConfig.json";
}

// =====================================================
// 工具函数
// =====================================================
static void ParseIDsFromText(const std::string& text)
{
    FileIDs.clear();
    const std::string key = "\"File_IDs\"";
    size_t keyPos = text.find(key);
    if (keyPos == std::string::npos)
        return;

    size_t lb = text.find('[', keyPos);
    size_t rb = text.find(']', lb);
    if (lb == std::string::npos || rb == std::string::npos)
        return;

    std::string inside = text.substr(lb + 1, rb - lb - 1);
    std::string cur;
    for (char ch : inside)
    {
        if (ch == ',' || ch == '\n')
        {
            cur.erase(std::remove_if(cur.begin(), cur.end(), [](unsigned char c) {
                return std::isspace(c) || c == '\"';
                }), cur.end());
            if (!cur.empty())
                FileIDs.push_back(cur);
            cur.clear();
        }
        else
        {
            cur.push_back(ch);
        }
    }
    cur.erase(std::remove_if(cur.begin(), cur.end(), [](unsigned char c) {
        return std::isspace(c) || c == '\"';
        }), cur.end());
    if (!cur.empty())
        FileIDs.push_back(cur);
}

bool LoadFileIDs()
{
    if (CONFIG_FILE.empty())
        CONFIG_FILE = FindWorkshopConfigPath();

    if (!fs::exists(CONFIG_FILE))
        return false;

    std::ifstream file(CONFIG_FILE);
    if (!file.is_open())
        return false;

    std::ostringstream ss;
    ss << file.rdbuf();
    file.close();

    ParseIDsFromText(ss.str());
    return true;
}

bool SaveFileIDs()
{
    if (CONFIG_FILE.empty())
        CONFIG_FILE = FindWorkshopConfigPath();

    // 先读取原始文件内容
    std::ifstream infile(CONFIG_FILE);
    std::string content;
    if (infile.is_open())
    {
        std::ostringstream ss;
        ss << infile.rdbuf();
        content = ss.str();
        infile.close();
    }

    // 找到 File_IDs 段
    size_t keyPos = content.find("\"File_IDs\"");
    if (keyPos != std::string::npos)
    {
        size_t lb = content.find('[', keyPos);
        size_t rb = content.find(']', lb);
        if (lb != std::string::npos && rb != std::string::npos)
        {
            // 构建新的 File_IDs 字符串
            std::ostringstream ids;
            ids << "[\n";
            for (size_t i = 0; i < FileIDs.size(); ++i)
            {
                ids << "    " << FileIDs[i];
                if (i + 1 < FileIDs.size())
                    ids << ",";
                ids << "\n";
            }
            ids << "  ]";

            // 替换原有 File_IDs
            content.replace(lb, rb - lb + 1, ids.str());
        }
    }
    else
    {
        // 如果文件里没有 File_IDs，创建新的 JSON 内容
        content = "{\n  \"File_IDs\": [\n";
        for (size_t i = 0; i < FileIDs.size(); ++i)
        {
            content += "    " + FileIDs[i];
            if (i + 1 < FileIDs.size()) content += ",";
            content += "\n";
        }
        content += "  ]\n}\n";
    }

    // 保存回文件
    std::ofstream outfile(CONFIG_FILE, std::ios::trunc);
    if (!outfile.is_open())
        return false;

    outfile << content;
    outfile.close();
    return true;
}

void AddFileID(const std::string& id)
{
    if (!id.empty())
        FileIDs.push_back(id);
}

void RemoveFileID(size_t index)
{
    if (index < FileIDs.size())
        FileIDs.erase(FileIDs.begin() + index);
}
