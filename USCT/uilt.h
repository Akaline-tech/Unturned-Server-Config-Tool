#pragma once

#include <string>
#include <vector>

struct WorkshopMod
{
    std::string id;
    std::string title;
    std::string author;
    std::string image;
};

std::string FindConfigPath(std::string filepath);