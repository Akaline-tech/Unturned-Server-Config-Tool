#pragma once
inline char sc_Name[64] = "Aria,server";
inline char sc_Password[13] = "6657";
inline char sc_Map[32] = "PEI";
inline char sc_Maxplayer[32] = "8";
inline int sc_Mode_item = 0;
inline bool sc_PVP = false;
inline char sc_Port[6] = "66570";
inline bool sc_Cheats = false;
inline int sc_Perspective_item = 0;
inline char sc_Loadout[64] = "255/";
inline char sc_Welcome[128] = "Power By UnturnedServerConfigTool";
inline char sc_Owner[32] = "";
inline char sc_GSLT[64] = "";

void LoadServerConfig();

void SaveServerConfig();