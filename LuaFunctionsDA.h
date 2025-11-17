/*	Lua DA Functions header
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2022 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#ifndef INCLUDE_LUAFUNCTIONS_DA
#define INCLUDE_LUAFUNCTIONS_DA
#include "LuaLib/lua.hpp"
#include "da_chatcommand.h"
#include <vector>
#include <string>

class LuaTT_DAChatCommand : public DAChatCommandClass
{
public:
	bool Activate(cPlayer* Player, const DATokenClass& Text, TextMessageEnum ChatType);
	void setupLua(std::string funcname, std::string com) { func = funcname; command = com; }
	std::string getFunc() { return func; }
	std::string getCommand() { return command; }
private:
	std::string func;
	std::string command;
};

void AddFunctionsDA(lua_State* L);
static std::vector<LuaTT_DAChatCommand*> LuaTT_DAChatList; // vector is faster than list

struct C4GameObjHax // Used for reading private members of C4GameObjHax
{
	byte offset[0x07B0 - 1]; // offets the memory
	float TriggerTime; // 07B0
	ReferencerClass Owner; // 07B4
	PlayerDataClass* Player; // 07C4
	AmmoDefinitionClass* AmmoDef; // 07C8
	int DetonationMode; // 07CC
	bool attached; // 07D0
	bool attachedToDynamic; // 07D1
	ReferencerClass attachObject; // 07D4
	Vector3 AttachLocation; // 07E4
	int AttachBoneIndex; // 07F0
	PhysClass* attachPhysics; // 07F4
	bool IsAttachedToMCT; // 07F8
	float SleepTime; // 07FC
};

#endif
