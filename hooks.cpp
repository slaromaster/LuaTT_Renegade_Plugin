/*	DA Hooks
Provides hooking of DA events to Lua env
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2022 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#include "General.h"
#include "Hooks.h"

_RequestSerial RequestSerial = 0;
_AddSerialHook AddSerialHook = 0;
_AddLoadingEHook AddLoadingEHook = 0;
_AddDamageHook AddDamageHook = 0;
_AddChatEHook AddChatEHook = 0;
_AddPingHook AddPingHook = 0;
_AddSuicideHook AddSuicideHook = 0;
//_AddRadioHook AddRadioHook = 0;

#pragma warning(disable: 4800)

#ifdef DAPLUGIN
#include "LuaLib\lua.hpp"
#include "LuaManager.h"
#include "ScriptZoneGameObj.h"
#include "da.h"
#include "da_player.h"

LuaTT_DA_Hooks::LuaTT_DA_Hooks()
{
	Register_Event(DAEvent::DALOG);  
	Register_Event(DAEvent::REFILL, INT_MAX);  
	Register_Event(DAEvent::SUICIDE, INT_MAX);  
	Register_Event(DAEvent::TEAMCHANGEREQUEST, INT_MAX);  
	Register_Event(DAEvent::ADDWEAPONREQUEST, INT_MAX);  
	Register_Event(DAEvent::BEACONDEPLOY, INT_MAX);  
	Register_Event(DAEvent::BEACONDETONATE, INT_MAX);  
	Register_Event(DAEvent::C4DETONATEREQUEST, INT_MAX);  
	Register_Event(DAEvent::CHARACTERPURCHASEREQUEST, INT_MAX);  
	Register_Event(DAEvent::CHATCOMMAND, INT_MAX);  
	Register_Event(DAEvent::CLEARWEAPONS, INT_MAX);  
	Register_Event(DAEvent::NAMECHANGE, INT_MAX);  
	Register_Event(DAEvent::HOSTCHAT, INT_MAX);  
	Register_Event(DAEvent::CUSTOMPURCHASEREQUEST, INT_MAX);  
	Register_Event(DAEvent::PLAYERLOADED, INT_MAX);  
	Register_Event(DAEvent::POWERUPGRANTREQUEST, INT_MAX);  
	Register_Event(DAEvent::POWERUPPURCHASEREQUEST, INT_MAX);  
	Register_Event(DAEvent::RADIO, INT_MAX);   
	Register_Event(DAEvent::REBALANCE, INT_MAX);  
	Register_Event(DAEvent::REMIX, INT_MAX);  
	Register_Event(DAEvent::REMOVEWEAPON, INT_MAX);  
	Register_Event(DAEvent::RENLOG, INT_MAX);  
	Register_Event(DAEvent::REQUESTVEHICLE, INT_MAX);  
	Register_Event(DAEvent::SWAP, INT_MAX);  
	Register_Event(DAEvent::VEHICLEENTRYREQUEST, INT_MAX);  
	Register_Event(DAEvent::VEHICLEFLIP, INT_MAX);  
	Register_Event(DAEvent::VEHICLEPURCHASEREQUEST, INT_MAX);  
	Register_Event(DAEvent::GAMEOVER);  
	Register_Event(DAEvent::KEYHOOK, INT_MAX);  
	Register_Event(DAEvent::CONNECTIONREQUEST, INT_MAX);  
	Register_Event(DAEvent::VEHICLEEXIT, INT_MAX);  
	Register_Event(DAEvent::SETTINGSLOADED, INT_MAX);
	Register_Event(DAEvent::VEHICLEPURCHASE, INT_MAX);
	Register_Event(DAEvent::ADDWEAPON, INT_MAX);
	Register_Event(DAEvent::C4DETONATE, INT_MAX);
	Register_Event(DAEvent::CHANGECHARACTER, INT_MAX);
	Register_Event(DAEvent::CHARACTERPURCHASE, INT_MAX);
	Register_Event(DAEvent::CHAT, INT_MAX);
	Register_Event(DAEvent::CONSOLEOUTPUT, INT_MAX);
	Register_Event(DAEvent::CUSTOMPURCHASE, INT_MAX);
	Register_Event(DAEvent::LEVELLOADED, INT_MAX);
	Register_Event(DAEvent::PLAYERJOIN, INT_MAX);
	Register_Event(DAEvent::PLAYERLEAVE, INT_MAX);
	Register_Event(DAEvent::PLAYERPREJOIN, INT_MAX);
	Register_Event(DAEvent::POWERUPGRANT, INT_MAX);
	Register_Event(DAEvent::POWERUPPURCHASE, INT_MAX);
	Register_Event(DAEvent::TEAMCHANGE, INT_MAX);
	Register_Event(DAEvent::VEHICLEENTER, INT_MAX);

	Register_Object_Event(DAObjectEvent::Type::ZONEEXIT, DAObjectEvent::ALL, INT_MAX);
	Register_Object_Event(DAObjectEvent::Type::ZONEENTER, DAObjectEvent::ALL, INT_MAX);
	Register_Object_Event(DAObjectEvent::Type::DAMAGERECEIVEDREQUEST, DAObjectEvent::ALL, INT_MAX);
	Register_Object_Event(DAObjectEvent::Type::CUSTOM, DAObjectEvent::ALL, INT_MAX);
	Register_Object_Event(DAObjectEvent::Type::POKE, DAObjectEvent::ALL, INT_MAX);
	Register_Object_Event(DAObjectEvent::Type::KILLRECEIVED, DAObjectEvent::ALL, INT_MAX);
	Register_Object_Event(DAObjectEvent::Type::CREATED, DAObjectEvent::ALL, INT_MAX);
	Register_Object_Event(DAObjectEvent::Type::DESTROYED, DAObjectEvent::ALL, INT_MAX);

}

void LuaTT_DA_Hooks::DA_Log_Event(const char *Header, const char *Output) 
{
	LuaManager::DA_Log_Event(Header, Output);
}

bool LuaTT_DA_Hooks::Refill_Event(cPlayer *Player)
{
	return LuaManager::DA_Refill_Event(Player); //Block all refills.
}

bool LuaTT_DA_Hooks::Suicide_Event(cPlayer *Player)
{
	return LuaManager::DA_Suicide_Event(Player); //Block suicides.
}

bool LuaTT_DA_Hooks::Team_Change_Request_Event(cPlayer *Player)
{
	return  LuaManager::DA_Team_Change_Request_Event(Player); //Block team change requests. This will catch both the team change button and the rtc command.
}


bool LuaTT_DA_Hooks::Chat_Command_Event(cPlayer *Player, TextMessageEnum Type, const StringClass &Command, const DATokenClass &Text, int ReceiverID)
{ 
	if (Player == nullptr)
	{
		return true;
	}
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;
	params.p4 = USE_STRING;
	params.p5 = USE_INT;
	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_Chat_Command_Event", params,
		(void *)Player->PlayerId,
		(void *)Type,
		(void *)Command.Peek_Buffer(),
		(void *)Text[0].Peek_Buffer(),
		(void *)ReceiverID,
		nullptr);
}

bool LuaTT_DA_Hooks::Chat_Event(cPlayer* Player, TextMessageEnum Type, const wchar_t* Message, int ReceiverID)
{
	if (Player == nullptr)
	{
		return true;
	}
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;
	params.p4 = USE_INT;
	params.p5 = USE_INT;
	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_Chat_Event", params,
		(void*)Player->PlayerId,
		(void*)Type,
		(void*)WideCharToChar(Message),
		(void*)ReceiverID,
		(void*)Player->Get_DA_Player()->Get_Access_Level(),
		nullptr);
}


bool LuaTT_DA_Hooks::Key_Hook_Event(cPlayer *Player, const StringClass &Key) 
{ 
	if (Player == nullptr)
	{
		return true;
	}
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_STRING;
	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_Key_Hook_Event", params, 
		(void *)Player->PlayerId, 
		(void *)Key.Peek_Buffer(),
		nullptr, nullptr, nullptr, nullptr);
}

bool LuaTT_DA_Hooks::Host_Chat_Event(int ID, TextMessageEnum Type, const char *Message)
{ 
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;
	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_Host_Chat_Event", params,
		(void *)ID,
		(void *)Type,
		(void *)Message,
		nullptr, nullptr, nullptr);
}

bool LuaTT_DA_Hooks::Radio_Event(cPlayer *Player, int PlayerType, int AnnouncementID, int IconID, AnnouncementEnum AnnouncementType)
{
	if (Player == nullptr)
	{
		return true;
	}
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_INT;
	params.p4 = USE_INT;
	params.p5 = USE_INT;
	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_Radio_Event", params,
		(void *)Player->PlayerId,
		(void *)PlayerType,
		(void *)AnnouncementID,
		(void *)IconID,
		(void *)AnnouncementType,
		nullptr);
}

ConnectionAcceptanceFilter::STATUS LuaTT_DA_Hooks::Connection_Request_Event(ConnectionRequest& Request, WideStringClass& RefusalMessage)
{
	ConnectionAcceptanceFilter::STATUS status;
	const char* refused;
	if (Request.clientSerialHash.Is_Empty()) // Called when old clients have not yet sent their serial numbers.
	{
		status = ConnectionAcceptanceFilter::STATUS_INDETERMINATE;
	}
	else
	{
		refused = LuaManager::DA_Connection_Request_Event(Request, RefusalMessage);
		RefusalMessage.Format((const wchar_t*)refused);
		if (strcmp(refused, "CONTINUE") == 0)
		{
			status = ConnectionAcceptanceFilter::STATUS_ACCEPTING;
		}
		else if (strcmp(refused, "INDETERMINATE") == 0)
		{
			status = ConnectionAcceptanceFilter::STATUS_INDETERMINATE;
		}
		else
		{
			RefusalMessage.Format(L"%S", (StringClass)refused);
			status = ConnectionAcceptanceFilter::STATUS_REFUSING;
		}
	}
	return status;
}

void LuaTT_DA_Hooks::Player_Pre_Join_Event(ConnectionRequest& Request)
{
	LuaManager::DA_Player_Pre_Join_Event(Request);
}

void LuaTT_DA_Hooks::Player_Loaded_Event(cPlayer *Player)
{ 
	if (Player == nullptr)
	{
		return;
	}
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_Player_Loaded_Event", params, 
		(void *)Player->PlayerId,
	nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Player_Join_Event(cPlayer* Player)
{
	if (Player == nullptr)
	{
		return;
	}
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_Player_Join_Event", params,
		(void*)Player->PlayerId,
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Player_Leave_Event(cPlayer* Player)
{
	if (Player == nullptr)
	{
		return;
	}
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_Player_Leave_Event", params,
		(void*)Player->PlayerId,
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Name_Change_Event(cPlayer *Player)
{ 
	if (Player == nullptr)
	{
		return;
	}
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_Name_Change_Event", params,
		(void *)Player->PlayerId,
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Remix_Event() 
{ 
	GenericParamList params;
	LuaManager::Generic_Hook("DA_Remix_Event", params, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Rebalance_Event()
{ 
	GenericParamList params;
	LuaManager::Generic_Hook("DA_Rebalance_Event", params, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Swap_Event() 
{ 
	GenericParamList params;
	LuaManager::Generic_Hook("DA_Swap_Event", params, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Game_Over_Event() 
{
	GenericParamList params;
	LuaManager::Generic_Hook("DA_Game_Over_Event", params, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Level_Loaded_Event()
{
	GenericParamList params;
	LuaManager::Generic_Hook("DA_Level_Loaded_Event", params, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Settings_Loaded_Event()
{
	GenericParamList params;
	LuaManager::Generic_Hook("DA_Settings_Loaded_Event", params, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Ren_Log_Event(const char *Output) 
{ 
	GenericParamList params;
	params.p1 = USE_STRING;
	LuaManager::Generic_Hook("DA_Ren_Log_Event", params,
		(void *)Output,
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Console_Output_Event(const char* Output)
{
	GenericParamList params;
	params.p1 = USE_STRING;
	LuaManager::Generic_Hook("DA_Console_Output_Event", params,
		(void*)Output,
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

PurchaseStatus LuaTT_DA_Hooks::Character_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, const SoldierGameObjDef* Item)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_FLOAT;
	params.p4 = USE_INT;
	params.p5 = USE_STRING;
	params.ret = USE_INT;
	params.defaultReturn = -1;
	return (PurchaseStatus)LuaManager::Generic_Hook("DA_Character_Purchase_Request_Event", params,
		(void *)Base->Get_Player_Type(),
		(void *)Player->PlayerId,
		(void *)&Cost,
		(void *)Item->Get_ID(), 
		(void *)Item->Get_Name(), 
		nullptr);
}

void LuaTT_DA_Hooks::Change_Character_Event(cPlayer* Player, const SoldierGameObjDef* Soldier)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;
	LuaManager::Generic_Hook("DA_Change_Character_Event", params,
		(void*)Player->PlayerId,
		(void*)Soldier->Get_ID(),
		(void*)Soldier->Get_Name(),
		nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Character_Purchase_Event(cPlayer* Player, float Cost, const SoldierGameObjDef* Item)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_FLOAT;
	params.p3 = USE_INT;
	params.p4 = USE_STRING;
	LuaManager::Generic_Hook("DA_Character_Purchase_Event", params,
		(void*)Player->PlayerId,
		(void*)&Cost,
		(void*)Item->Get_ID(),
		(void*)Item->Get_Name(),
		nullptr, nullptr);
}

PurchaseStatus LuaTT_DA_Hooks::Vehicle_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, const VehicleGameObjDef* Item)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_FLOAT;
	params.p4 = USE_INT;
	params.p5 = USE_STRING;
	params.ret = USE_INT;
	params.defaultReturn = -1;
	return (PurchaseStatus)LuaManager::Generic_Hook("DA_Vehicle_Purchase_Request_Event", params,
		(void *)Base->Get_Player_Type(),
		(void *)Player->PlayerId,
		(void *)&Cost,
		(void *)Item->Get_ID(),
		(void *)Item->Get_Name(),
		nullptr);
}

void LuaTT_DA_Hooks::Vehicle_Purchase_Event(cPlayer* Player, float Cost, const VehicleGameObjDef* Item)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_FLOAT;
	params.p3 = USE_INT;
	params.p4 = USE_STRING;
	LuaManager::Generic_Hook("DA_Vehicle_Purchase_Event", params,
		(void*)Player->PlayerId,
		(void*)&Cost,
		(void*)Item->Get_ID(),
		(void*)Item->Get_Name(),
		nullptr, nullptr);
}

PurchaseStatus LuaTT_DA_Hooks::PowerUp_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, const PowerUpGameObjDef* Item)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_FLOAT;
	params.p4 = USE_INT;
	params.p5 = USE_STRING;
	params.ret = USE_INT;
	params.defaultReturn = -1;
	return (PurchaseStatus)LuaManager::Generic_Hook("DA_PowerUp_Purchase_Request_Event", params,
		(void *)Base->Get_Player_Type(),
		(void *)Player->PlayerId,
		(void *)&Cost,
		(void *)Item->Get_ID(),
		(void *)Item->Get_Name(),
		nullptr);
}

void LuaTT_DA_Hooks::PowerUp_Purchase_Event(cPlayer* Player, float Cost, const PowerUpGameObjDef* Item)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_FLOAT;
	params.p3 = USE_INT;
	params.p4 = USE_STRING;

	LuaManager::Generic_Hook("DA_PowerUp_Purchase_Event", params,
		(void*)Player->PlayerId,
		(void*)&Cost,
		(void*)Item->Get_ID(),
		(void*)Item->Get_Name(),
		nullptr, nullptr);
}

PurchaseStatus LuaTT_DA_Hooks::Custom_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, unsigned int ID)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_FLOAT;
	params.p4 = USE_INT;
	params.ret = USE_INT;
	params.defaultReturn = 3;
	return (PurchaseStatus)LuaManager::Generic_Hook("DA_Custom_Purchase_Request_Event", params,
		(void *)Base->Get_Player_Type(),
		(void *)Player->PlayerId,
		(void *)&Cost,
		(void *)ID,
		nullptr, nullptr);
}

void LuaTT_DA_Hooks::Custom_Purchase_Event(cPlayer* Player, float Cost, unsigned int ID)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_FLOAT;
	params.p3 = USE_INT;
	LuaManager::Generic_Hook("DA_Custom_Purchase_Event", params,
		(void*)Player->PlayerId,
		(void*)&Cost,
		(void*)ID,
		nullptr, nullptr, nullptr);
}


void LuaTT_DA_Hooks::Team_Change_Event(cPlayer *Player)
{
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_Team_Change_Event", params,
		(void *)Player->PlayerId,
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

bool LuaTT_DA_Hooks::Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle, cPlayer *Player, int &Seat)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_INT;
	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_Vehicle_Entry_Request_Event", params,
		(void *)Vehicle->Get_ID(),
		(void *)Player->PlayerId,
		(void *)Seat,
		nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Vehicle_Enter_Event(VehicleGameObj* Vehicle, cPlayer* Player, int Seat)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_INT;
	LuaManager::Generic_Hook("DA_Vehicle_Enter_Event", params,
		(void*)Vehicle->Get_ID(),
		(void*)Player->PlayerId,
		(void*)Seat,
		nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Vehicle_Exit_Event(VehicleGameObj *Vehicle, cPlayer *Player, int Seat)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_INT;
	LuaManager::Generic_Hook("DA_Vehicle_Exit_Event", params,
		(void *)Vehicle->Get_ID(),
		(void *)Player->PlayerId,
		(void *)Seat,
		nullptr, nullptr, nullptr);
}

bool LuaTT_DA_Hooks::PowerUp_Grant_Request_Event(cPlayer *Player, const PowerUpGameObjDef *PowerUp, PowerUpGameObj *PowerUpObj)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;
	params.p4 = USE_INT;

	int PowerupId = 0;
	if (PowerUpObj != nullptr)
	{
		PowerupId = PowerUpObj->Get_ID();
	}

	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_PowerUp_Grant_Request_Event", params,
		(void *)Player->PlayerId,
		(void *)PowerUp->Get_ID(),
		(void *)PowerUp->Get_Name(),
		(void *)PowerupId,
		nullptr, nullptr);
}

void LuaTT_DA_Hooks::PowerUp_Grant_Event(cPlayer* Player, const PowerUpGameObjDef* PowerUp, PowerUpGameObj* PowerUpObj)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;
	params.p4 = USE_INT;

	int PowerupId = 0;
	if (PowerUpObj != nullptr)
	{
		PowerupId = PowerUpObj->Get_ID();
	}

	LuaManager::Generic_Hook("DA_PowerUp_Grant_Event", params,
		(void*)Player->PlayerId,
		(void*)PowerUp->Get_ID(),
		(void*)PowerUp->Get_Name(),
		(void*)PowerupId,
		nullptr, nullptr);
}

bool LuaTT_DA_Hooks::Add_Weapon_Request_Event(cPlayer *Player, const WeaponDefinitionClass *Weapon)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;
	params.ret = USE_BOOL;
	params.defaultReturn = true;

	int WeaponId = 0;
	const char *WeaponName = "None";
	if (Weapon != nullptr)
	{
		WeaponId = Weapon->Get_ID();
		WeaponName = Weapon->Get_Name();

	}
	int PlayerID = 0;
	if (Player != nullptr)
	{
		PlayerID = Player->PlayerId;
	}

	return LuaManager::Generic_Hook("DA_Add_Weapon_Request_Event", params,
		(void *)PlayerID,
		(void *)WeaponId,
		(void *)WeaponName,
		nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Add_Weapon_Event(cPlayer* Player, WeaponClass* Weapon)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;

	int WeaponId = 0;
	const char* WeaponName = "None";
	if (Weapon != nullptr)
	{
		WeaponId = Weapon->Get_ID();
		WeaponName = Weapon->Get_Name();

	}
	int PlayerID = 0;
	if (Player != nullptr)
	{
		PlayerID = Player->PlayerId;
	}

	LuaManager::Generic_Hook("DA_Add_Weapon_Event", params,
		(void*)PlayerID,
		(void*)WeaponId,
		(void*)WeaponName,
		nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Remove_Weapon_Event(cPlayer *Player, WeaponClass *Weapon)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_STRING;

	int WeaponId = 0;
	const char* WeaponName = "None";
	if (Weapon != nullptr)
	{
		WeaponId = Weapon->Get_ID();
		WeaponName = Weapon->Get_Name();

	}
	int PlayerID = 0;
	if (Player != nullptr)
	{
		PlayerID = Player->PlayerId;
	}

	LuaManager::Generic_Hook("DA_Remove_Weapon_Event", params,
		(void*)PlayerID,
		(void*)WeaponId,
		(void*)WeaponName,
		nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Clear_Weapons_Event(cPlayer *Player)
{
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_Clear_Weapons_Event", params,
		(void *)Player->PlayerId, 
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Beacon_Deploy_Event(BeaconGameObj *Beacon)
{
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_Beacon_Deploy_Event", params,
		(void *)Beacon->Get_ID(),
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Beacon_Detonate_Event(BeaconGameObj *Beacon)
{
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_Beacon_Detonate_Event", params,
		(void *)Beacon->Get_ID(),
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

bool LuaTT_DA_Hooks::C4_Detonate_Request_Event(C4GameObj *C4, SmartGameObj *Triggerer)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	
	int TriggerID = 0;
	if (Triggerer != nullptr)
	{
		TriggerID = Triggerer->Get_ID();
	}

	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_C4_Detonate_Request_Event", params,
		(void *)C4->Get_ID(),
		(void *)TriggerID,
		nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::C4_Detonate_Event(C4GameObj* C4)
{
	GenericParamList params;
	params.p1 = USE_INT;
	LuaManager::Generic_Hook("DA_C4_Detonate_Event", params,
		(void*)C4->Get_ID(),
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

bool LuaTT_DA_Hooks::Vehicle_Flip_Event(VehicleGameObj *Vehicle)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.ret = USE_BOOL;
	params.defaultReturn = true;
	return LuaManager::Generic_Hook("DA_Vehicle_Flip_Event", params,
		(void *)Vehicle->Get_ID(),
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

bool LuaTT_DA_Hooks::Request_Vehicle_Event(VehicleFactoryGameObj *Factory, const VehicleGameObjDef *Vehicle, cPlayer *Player, float Delay)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_INT;
	params.p4 = USE_FLOAT;
	params.ret = USE_BOOL;
	params.defaultReturn = true;

	int PlayerID = 0;
	if (Player != nullptr)
	{
		PlayerID = Player->PlayerId;
	}

	return LuaManager::Generic_Hook("DA_Request_Vehicle_Event", params,
		(void *)Factory->Get_ID(),
		(void *)Vehicle->Get_ID(),
		(void *)PlayerID,
		(void *)&Delay,
		nullptr, nullptr);
}

bool LuaTT_DA_Hooks::Damage_Request_Event(DamageableGameObj *Victim, ArmedGameObj *Damager, float &Damage, unsigned int &Warhead, float Scale, DADamageType::Type Type)
{
	return LuaManager::DA_Damage_Request_Event(Victim, Damager, Damage, Warhead, Scale, Type);
}

void LuaTT_DA_Hooks::Kill_Event(DamageableGameObj *Victim, ArmedGameObj *Killer, float Damage, unsigned int Warhead, float Scale, DADamageType::Type Type)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_FLOAT;
	params.p4 = USE_INT;
	params.p5 = USE_FLOAT;
	params.p6 = USE_INT;

	int VictimID = 0;
	if (Victim != nullptr)
	{
		VictimID = Victim->Get_ID();
	}

	int KillerID = 0;
	if (Killer != nullptr)
	{
		KillerID = Killer->Get_ID();
	}

	LuaManager::Generic_Hook("DA_Kill_Event", params,
		(void *)VictimID,
		(void *)KillerID,
		(void *)&Damage,
		(void *)Warhead,
		(void *)&Scale,
		(void *)Type);
}

void LuaTT_DA_Hooks::Custom_Event(GameObject *obj, int Type, int Param, GameObject *Sender)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;
	params.p3 = USE_INT;
	params.p4 = USE_INT;

	int SenderID = 0;
	if (Sender != nullptr)
	{
		SenderID = Sender->Get_ID();
	}

	LuaManager::Generic_Hook("DA_Custom_Event", params,
		(void *)obj->Get_ID(),
		(void *)Type,
		(void *)Param,
		(void *)SenderID,
		nullptr, nullptr);
}

void LuaTT_DA_Hooks::Poke_Event(cPlayer *Player, PhysicalGameObj *obj)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;

	LuaManager::Generic_Hook("DA_Poke_Event", params,
		(void *)Player->PlayerId,
		(void *)obj->Get_ID(),
		nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Zone_Enter_Event(ScriptZoneGameObj *obj, PhysicalGameObj *Enterer)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;

	int EntererID = 0;
	if (Enterer != nullptr)
	{
		EntererID = Enterer->Get_ID();
	}

	LuaManager::Generic_Hook("DA_Zone_Enter_Event", params,
		(void *)obj->Get_ID(),
		(void *)EntererID,
		nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Zone_Exit_Event(ScriptZoneGameObj *obj, PhysicalGameObj *Exiter)
{
	GenericParamList params;
	params.p1 = USE_INT;
	params.p2 = USE_INT;

	int ExiterID = 0;
	if (Exiter != nullptr)
	{
		ExiterID = Exiter->Get_ID();
	}

	LuaManager::Generic_Hook("DA_Zone_Exit_Event", params,
		(void *)obj->Get_ID(),
		(void *)ExiterID,
		nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Object_Created_Event(GameObject* obj)
{
	GenericParamList params;
	params.p1 = USE_INT;

	LuaManager::Generic_Hook("DA_Object_Created_Event", params,
		(void*)obj->Get_ID(),
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

void LuaTT_DA_Hooks::Object_Destroyed_Event(GameObject* obj)
{
	GenericParamList params;
	params.p1 = USE_INT;

	LuaManager::Generic_Hook("DA_Object_Destroyed_Event", params,
		(void*)obj->Get_ID(),
		nullptr, nullptr, nullptr, nullptr, nullptr);
}

/**********************
******DA Crate*********
***********************/

template <typename T> class LuaTT_DACrateRegistrant : public DACrateFactoryClass {
public:
	LuaTT_DACrateRegistrant(const char *Name, DACrateType::Type Type, lua_State *L_param, int uRef_param) : DACrateFactoryClass(Name, Type), m_name(Name), L(L_param), uRef(uRef_param) {}
	virtual DACrateClass *Create_Instance() {
		if (!Instance) {
			Instance = new T(L, uRef);
			Instance->Set_Factory(this);
			Instance->Init();
		}
		return Instance;
	};
	virtual ~LuaTT_DACrateRegistrant() 
	{
		delete m_name;
	}
private:
	const char *m_name;
	lua_State *L;
	int uRef = 0;
};

static std::vector<LuaTT_DACrateRegistrant<LuaTT_DACrateClass>*> crateList;

int LuaTT_DACrateClass::Lua_Register_Crate(lua_State *L)
{
	if (lua_gettop(L) < 3) return 0;

	if (!lua_istable(L, -1))
	{
		luaL_error(L, "Bad argument #1 to DA_Register_Crate. Expected table.");
		return 0;
	}

	// TODO check if already exists

	const char *Name = luaL_checkstring(L, -2);
	if (!Name)
	{
		luaL_error(L, "Bad argument #2 to DA_Register_Crate. Expected Crate Name (string).");
		return 0;
	}

	int type = luaL_checkinteger(L, -3);
	int uRef = luaL_ref(L, LUA_REGISTRYINDEX);
	const char *CloneName = _strdup(Name);
	LuaTT_DACrateRegistrant<LuaTT_DACrateClass> *obj = new LuaTT_DACrateRegistrant<LuaTT_DACrateClass>(CloneName, static_cast<DACrateType::Type>(type), L, uRef);
	crateList.push_back(obj);
	return 0;
}

// TODO Unregister_Crate? 

LuaTT_DACrateClass::LuaTT_DACrateClass(lua_State *L_param, int uRef_param)
: L(L_param)
, uRef(uRef_param)
{
}

void LuaTT_DACrateClass::Init()
{
	if (L == nullptr)
		return;
	
	lua_rawgeti(L, LUA_REGISTRYINDEX, uRef);
	if (!lua_istable(L, -1))
	{
		return;
	}

	lua_getfield(L, -1, "Init");
	if (lua_isfunction(L, -1))
	{
		lua_pushvalue(L, -2);
		LuaManager::Report_Errors(L, lua_pcall(L, 1, 0, 0));
	}
	lua_pop(L, -1);
}

void LuaTT_DACrateClass::Activate(cPlayer *Player)
{
	if (L == nullptr)
		return;

	if (!Player)
	{
		return;
	}

	lua_rawgeti(L, LUA_REGISTRYINDEX, uRef);
	if (!lua_istable(L, -1))
	{
		return;
	}

	lua_getfield(L, -1, "Activate");
	if (lua_isfunction(L, -1))
	{
		lua_pushvalue(L, -2);
		lua_pushnumber(L, Player->Get_Id());
		LuaManager::Report_Errors(L, lua_pcall(L, 2, 0, 0));
	}
	lua_pop(L, -1);
}


bool LuaTT_DACrateClass::Can_Activate(cPlayer *Player)
{
	if (L == nullptr)
		return false;

	if (!Player)
	{
		return false;
	}
	
	bool ret = true;

	lua_rawgeti(L, LUA_REGISTRYINDEX, uRef);
	if (!lua_istable(L, -1))
	{
		return false;
	}
	
	lua_getfield(L, -1, "Can_Activate");
	if (lua_isfunction(L, -1))
	{
		lua_pushvalue(L, -2);
		lua_pushnumber(L, Player->Get_Id());
		LuaManager::Report_Errors(L, lua_pcall(L, 2, 1, 0));
		ret = lua_toboolean(L, -1);
	}
	lua_pop(L, -1);

	return ret;
}


/**********************
****DA GameFeature*****
***********************/

template <class T> class LuaTT_DAConsoleFunctionRegistrant {
public:
	LuaTT_DAConsoleFunctionRegistrant(const char *name, const char *alias, const char *help, lua_State *L_param, int uRef_param) : m_name(name), m_alias(alias), m_help(help), L(L_param), uRef(uRef_param) {
		T *obj = new T(L, uRef, m_name, m_alias, m_help);
		DA::Get_Console_Function_List().Add(obj);
	}

	virtual ~LuaTT_DAConsoleFunctionRegistrant()
	{
		delete m_name;
		delete m_help;
		delete m_alias;
	}
private:
	const char *m_name;
	const char *m_alias;
	const char *m_help;
	lua_State *L;
	int uRef = 0;
};


static std::vector<LuaTT_DAConsoleFunctionRegistrant<LuaTT_DAConsoleFunction>*> consoleList;

int LuaTT_DAConsoleFunction::Lua_Register_Console_Function(lua_State *L)
{
	if (lua_gettop(L) < 4) return 0;

	if (!lua_istable(L, -1))
	{
		luaL_error(L, "Bad argument #4 to DA_Register_Console_Function. Expected table.");
		return 0;
	}

	const char *Name = luaL_checkstring(L, -4);
	if (!Name)
	{
		luaL_error(L, "Bad argument #1 to DA_Register_Console_Function. Expected Console Name (string).");
		return 0;
	}

	const char *Alias = luaL_checkstring(L, -3);
	if (!Alias)
	{
		luaL_error(L, "Bad argument #2 to DA_Register_Console_Function. Expected Console Alias (string).");
		return 0;
	}

	const char *Help = luaL_checkstring(L, -2);
	if (!Help)
	{
		luaL_error(L, "Bad argument #3 to DA_Register_Console_Function. Expected Console Help (string).");
		return 0;
	}

	int uRef = luaL_ref(L, LUA_REGISTRYINDEX);
	const char *CloneName = _strdup(Name);
	const char *CloneAlias = _strdup(Alias);
	const char *CloneHelp = _strdup(Help);
	LuaTT_DAConsoleFunctionRegistrant<LuaTT_DAConsoleFunction> *obj = new LuaTT_DAConsoleFunctionRegistrant<LuaTT_DAConsoleFunction>(CloneName, CloneAlias, CloneHelp, L, uRef);
	consoleList.push_back(obj);
	return 0;
}

LuaTT_DAConsoleFunction::LuaTT_DAConsoleFunction(lua_State *L_param, int uRef_param, const char *name, const char *alias, const char *help)
: L(L_param)
, uRef(uRef_param)
, m_name(name)
, m_alias(alias)
, m_help(help)
{
}

void LuaTT_DAConsoleFunction::Activate(const char *ArgumentsString)
{
	
	if (L == nullptr)
		return;
	
	lua_rawgeti(L, LUA_REGISTRYINDEX, uRef);
	if (!lua_istable(L, -1))
	{
		return;
	}

	lua_getfield(L, -1, "Activate");
	if (lua_isfunction(L, -1))
	{
		lua_pushvalue(L, -2);
		lua_pushstring(L, ArgumentsString);
		LuaManager::Report_Errors(L, lua_pcall(L, 2, 0, 0));
	}
	lua_pop(L, -1);
}

#endif