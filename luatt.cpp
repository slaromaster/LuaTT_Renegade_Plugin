/*	LuaTT plugin init
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2022 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#include "General.h"
#include "luatt.h"
#include "plugin.h"
#include "engine_tt.h"
#include "engine_io.h"
#include "hooks.h"

#ifndef DAPLUGIN
#include "gmgame.h"
#else
#include "da_ssgm.h"
#include "engine_da.h"
#include "da.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_vehicle.h"
#endif

#include "AABoxClass.h"

#include "LuaLib\lua.hpp"
#include "LuaManager.h"
#include "LuaThread.h"

#include "CommandLineParser.h"
#include "ConnectionRequest.h"


luatt::luatt()
{
#ifdef DAPLUGIN
	daHooks = new LuaTT_DA_Hooks;
#endif
	RegisterEvent(EVENT_GLOBAL_INI,this);
	RegisterEvent(EVENT_MAP_INI,this);
	RegisterEvent(EVENT_CHAT_HOOK,this);
	RegisterEvent(EVENT_OBJECT_CREATE_HOOK,this);
	RegisterEvent(EVENT_LOAD_LEVEL_HOOK,this);
	RegisterEvent(EVENT_GAME_OVER_HOOK,this);
	RegisterEvent(EVENT_PLAYER_JOIN_HOOK,this);
	RegisterEvent(EVENT_PLAYER_LEAVE_HOOK,this);
	RegisterEvent(EVENT_REFILL_HOOK,this);
	RegisterEvent(EVENT_POWERUP_PURCHASE_HOOK,this);
	RegisterEvent(EVENT_VEHICLE_PURCHASE_HOOK,this);
	RegisterEvent(EVENT_CHARACTER_PURCHASE_HOOK,this);
	RegisterEvent(EVENT_THINK_HOOK,this);
#ifdef DAPLUGIN
	AddDialogHook(LuaManager::Call_On_Dialog);
#else
	RegisterEvent(EVENT_DIALOG_HOOK, this);
#endif

	// hooks
	AddHostHook(LuaManager::Call_Host_Hook);
	addConnectionAcceptanceFilter(this);

	LuaLoad();

	AddCharacterPurchaseMonHook(LuaManager::Purchase_Hook, "OnCharacterPurchase");
	AddVehiclePurchaseMonHook(LuaManager::Purchase_Hook, "OnVehiclePurchase");
	AddPowerupPurchaseMonHook(LuaManager::Purchase_Hook, "OnPowerupPurchase");

	ObjectCreateHookStruct *_och = new ObjectCreateHookStruct;
	_och->hook = LuaManager::Call_Object_Hook;
	_och->data = 0;
	AddObjectCreateHook(_och);
}


void luatt::handleInitiation(const ConnectionRequest& connectionRequest)
{
}
	
void luatt::handleTermination(const ConnectionRequest& connectionRequest)
{
}
	
void luatt::handleCancellation(const ConnectionRequest& connectionRequest)
{
}
	
ConnectionAcceptanceFilter::STATUS luatt::getStatus(const ConnectionRequest& connectionRequest, WideStringClass& refusalMessage)
{
	STATUS status;
	const char *refused;
	if (connectionRequest.clientSerialHash.Is_Empty()) // Called when old clients have not yet sent their serial numbers.
		status = STATUS_INDETERMINATE;
	else
	{
		// code goes here

		
			// get ip
	    char ip[40];
		char serialp[55];
		char version[55];
		char nick[55];

        int ip1,ip2,ip3,ip4;
        ip1 = connectionRequest.clientAddress.sin_addr.s_addr&0x000000FF;
        ip2 = (connectionRequest.clientAddress.sin_addr.s_addr&0x0000FF00)>>8;
        ip3 = (connectionRequest.clientAddress.sin_addr.s_addr&0x00FF0000)>>16;
        ip4 = (connectionRequest.clientAddress.sin_addr.s_addr&0xFF000000)>>24;

       sprintf(serialp, "%s" ,connectionRequest.clientSerialHash);
       sprintf(ip,"%d.%d.%d.%d",ip1,ip2,ip3,ip4);
	   sprintf(nick, "%ls", connectionRequest.clientName);
	   sprintf(version, "%f", connectionRequest.clientVersion);
		refused = LuaManager::ConAc((const char*)ip, (const char*)nick, (const char*)serialp, (const char*)version, connectionRequest.clientId);
		refusalMessage.Format((const wchar_t *)refused);

		if (strcmp(refused, "CONTINUE") == 0) {
			status = STATUS_ACCEPTING;
		}
		else if (strcmp(refused, "INDETERMINATE") == 0) {
			status = STATUS_INDETERMINATE;
		}
		else {
			refusalMessage.Format(L"%S", (StringClass)refused);
			status = STATUS_REFUSING;
		}
	 
	}
	return status;
}

luatt::~luatt()
{
	LuaScriptManager::Cleanup();
	LuaManager::UnloadingLua();
	LuaManager::Cleanup();
	UnregisterEvent(EVENT_GLOBAL_INI,this);
	UnregisterEvent(EVENT_MAP_INI,this);
	UnregisterEvent(EVENT_CHAT_HOOK,this);
	UnregisterEvent(EVENT_OBJECT_CREATE_HOOK,this);
	UnregisterEvent(EVENT_LOAD_LEVEL_HOOK,this);
	UnregisterEvent(EVENT_GAME_OVER_HOOK,this);
	UnregisterEvent(EVENT_PLAYER_JOIN_HOOK,this);
	UnregisterEvent(EVENT_PLAYER_LEAVE_HOOK,this);
	UnregisterEvent(EVENT_REFILL_HOOK,this);
	UnregisterEvent(EVENT_POWERUP_PURCHASE_HOOK,this);
	UnregisterEvent(EVENT_VEHICLE_PURCHASE_HOOK,this);
	UnregisterEvent(EVENT_CHARACTER_PURCHASE_HOOK,this);
	UnregisterEvent(EVENT_THINK_HOOK,this);
#ifndef DAPLUGIN
	UnregisterEvent(EVENT_DIALOG_HOOK, this);
#endif
	removeConnectionAcceptanceFilter(this);
#ifdef DAPLUGIN
	delete daHooks;
#endif
}

bool luatt::OnChat(int PlayerID,TextMessageEnum Type,const wchar_t *Message,int recieverID)
{
	int ret = LuaManager::Call_Chat_Hook(PlayerID, Type, Message, recieverID);
	return ret == 0 ? false : true;
}

void luatt::OnObjectCreate(void *data,GameObject *obj)
{
	/* Not used */
}

void luatt::OnDialog(int PlayerID, int DialogID, int ControlID, DialogMessageType MessageType)
{
	LuaManager::Call_On_Dialog(PlayerID, DialogID, ControlID, MessageType);
}


void luatt::OnLoadLevel()
{
	LuaManager::Call_Level_Loaded_Hook();
}

void luatt::OnGameOver()
{
	LuaManager::Call_GameOver_Hook();
}

void luatt::OnPlayerJoin(int PlayerID,const char *PlayerName)
{
	if (!PlayerID) { return; }
	if (!PlayerName) { return; }
	LuaManager::Call_Player_Join_Hook(PlayerID, PlayerName);
}

void luatt::OnPlayerLeave(int PlayerID)
{
	if (!PlayerID) { return; }
	LuaManager::Call_Player_Leave_Hook(PlayerID);
}

bool luatt::OnRefill(GameObject *purchaser)
{ 
 if (!purchaser) { return true; }
 int ret = LuaManager::Refill_Hook(purchaser);
 return ret == 0 ? false : true;
  //return true;
}

PurchaseStatus luatt::OnPowerupPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	//LuaManager::Purchase_Hook(base,purchaser,cost,preset,data);
	return PurchaseStatus_Allow;
}

PurchaseStatus luatt::OnVehiclePurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
	PurchaseStatus i = (PurchaseStatus)LuaManager::Purchase_Hook2(base, purchaser, cost, preset, data);
	if (!i) { return PurchaseStatus_Allow; }
	return i;
}

PurchaseStatus luatt::OnCharacterPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data)
{
    //LuaManager::Purchase_Hook(base,purchaser,cost,preset,data);
	return PurchaseStatus_Allow;
}

void luatt::OnThink()
{
	if (!LuaTT_threadEnabled)
		LuaTT_threadEnabled = true;
	LuaManager::Call_Think_Hook();
}

luatt luaplugin;

extern "C" __declspec(dllexport) Plugin* Plugin_Init()
{
	return &luaplugin;
}


