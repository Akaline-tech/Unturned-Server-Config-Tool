// ModConfig.h
#pragma once
#include <vector>
#include <string>


extern std::vector<std::string> FileIDs;

std::string FindWorkshopConfigPath();
std::string GetConfigPath();
bool LoadFileIDs();
bool SaveFileIDs();
void AddFileID(const std::string& id);
void RemoveFileID(size_t index);
