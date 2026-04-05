#include "ModConfig.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include "uilt.h"

namespace fs = std::filesystem;

std::vector<std::string> FileIDs;
static std::string CONFIG_FILE;

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
        CONFIG_FILE = FindConfigPath("\\steamapps\\common\\U3DS\\Servers\\Default\\WorkshopDownloadConfig.json");

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
        CONFIG_FILE = FindConfigPath("\\steamapps\\common\\U3DS\\Servers\\Default\\WorkshopDownloadConfig.json");

    std::ifstream infile(CONFIG_FILE);
    std::string content;
    if (infile.is_open())
    {
        std::ostringstream ss;
        ss << infile.rdbuf();
        content = ss.str();
        infile.close();
    }

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

            content.replace(lb, rb - lb + 1, ids.str());
        }
    }
    else
    {
        content = "{\n  \"File_IDs\": [\n";
        for (size_t i = 0; i < FileIDs.size(); ++i)
        {
            content += "    " + FileIDs[i];
            if (i + 1 < FileIDs.size()) content += ",";
            content += "\n";
        }
        content += "  ]\n}\n";
    }

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

