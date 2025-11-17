/*	DA Hooks header
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2022 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#ifndef INCLUDE_HOOKS
#define INCLUDE_HOOKS

#include "General.h"
#include "luatt.h"

#ifdef DAPLUGIN

#include "da_event.h" //For DAEventClass definition.
#include "da_crate.h"
#include "da_gamefeature.h"


//Inherit from DAEventClass.
class LuaTT_DA_Hooks : public DAEventClass {
public:
	LuaTT_DA_Hooks();
	virtual void DA_Log_Event(const char *Header, const char *Output);
	virtual bool Refill_Event(cPlayer *Player);
	virtual bool Suicide_Event(cPlayer *Player);
	virtual bool Team_Change_Request_Event(cPlayer *Player);

	virtual bool Chat_Command_Event(cPlayer *Player, TextMessageEnum Type, const StringClass &Command, const DATokenClass &Text, int ReceiverID);
	virtual bool Chat_Event(cPlayer* Player, TextMessageEnum Type, const wchar_t* Message, int ReceiverID);
	virtual bool Key_Hook_Event(cPlayer *Player, const StringClass &Key);
	virtual bool Host_Chat_Event(int ID, TextMessageEnum Type, const char *Message);
	virtual bool Radio_Event(cPlayer *Player, int PlayerType, int AnnouncementID, int IconID, AnnouncementEnum AnnouncementType);
	virtual ConnectionAcceptanceFilter::STATUS Connection_Request_Event(ConnectionRequest& Request, WideStringClass& RefusalMessage);
	virtual void Player_Pre_Join_Event(ConnectionRequest& Request);
	virtual void Player_Loaded_Event(cPlayer *Player);
	virtual void Player_Join_Event(cPlayer* Player);
	virtual void Player_Leave_Event(cPlayer* Player);
	virtual void Name_Change_Event(cPlayer *Player);
	virtual void Remix_Event();
	virtual void Rebalance_Event();
	virtual void Swap_Event();
	virtual void Game_Over_Event();
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();
	virtual void Ren_Log_Event(const char *Output);
	virtual void Console_Output_Event(const char* Output);
	virtual PurchaseStatus Character_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, const SoldierGameObjDef* Item);
	virtual void Change_Character_Event(cPlayer* Player, const SoldierGameObjDef* Soldier);
	virtual void Character_Purchase_Event(cPlayer* Player, float Cost, const SoldierGameObjDef* Item);
	virtual PurchaseStatus Vehicle_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, const VehicleGameObjDef* Item);
	virtual void Vehicle_Purchase_Event(cPlayer* Player, float Cost, const VehicleGameObjDef* Item);
	virtual PurchaseStatus PowerUp_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, const PowerUpGameObjDef* Item);
	virtual void PowerUp_Purchase_Event(cPlayer* Player, float Cost, const PowerUpGameObjDef* Item);
	virtual PurchaseStatus Custom_Purchase_Request_Event(BaseControllerClass* Base, cPlayer* Player, float& Cost, unsigned int ID);
	virtual void Custom_Purchase_Event(cPlayer* Player, float Cost, unsigned int ID);
	virtual void Team_Change_Event(cPlayer *Player);
	virtual bool Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle, cPlayer *Player, int &Seat);
	virtual void Vehicle_Enter_Event(VehicleGameObj* Vehicle, cPlayer* Player, int Seat);
	virtual void Vehicle_Exit_Event(VehicleGameObj *Vehicle, cPlayer *Player, int Seat);
	virtual bool PowerUp_Grant_Request_Event(cPlayer *Player, const PowerUpGameObjDef *PowerUp, PowerUpGameObj *PowerUpObj);
	virtual void PowerUp_Grant_Event(cPlayer* Player, const PowerUpGameObjDef* PowerUp, PowerUpGameObj* PowerUpObj);
	virtual bool Add_Weapon_Request_Event(cPlayer *Player, const WeaponDefinitionClass *Weapon);
	virtual void Add_Weapon_Event(cPlayer* Player, WeaponClass* Weapon);
	virtual void Remove_Weapon_Event(cPlayer *Player, WeaponClass *Weapon);
	virtual void Clear_Weapons_Event(cPlayer *Player);
	virtual void Beacon_Deploy_Event(BeaconGameObj *Beacon);
	virtual void Beacon_Detonate_Event(BeaconGameObj *Beacon);
	virtual bool C4_Detonate_Request_Event(C4GameObj *C4, SmartGameObj *Triggerer);
	virtual void C4_Detonate_Event(C4GameObj* C4);
	virtual bool Vehicle_Flip_Event(VehicleGameObj *Vehicle);
	virtual bool Request_Vehicle_Event(VehicleFactoryGameObj *Factory, const VehicleGameObjDef *Vehicle, cPlayer *Player, float Delay);

	virtual bool Damage_Request_Event(DamageableGameObj *Victim, ArmedGameObj *Damager, float &Damage, unsigned int &Warhead, float Scale, DADamageType::Type Type);
	virtual void Kill_Event(DamageableGameObj *Victim, ArmedGameObj *Killer, float Damage, unsigned int Warhead, float Scale, DADamageType::Type Type);
	virtual void Custom_Event(GameObject *obj, int Type, int Param, GameObject *Sender);
	virtual void Poke_Event(cPlayer *Player, PhysicalGameObj *obj);
	virtual void Zone_Enter_Event(ScriptZoneGameObj *obj, PhysicalGameObj *Enterer);
	virtual void Zone_Exit_Event(ScriptZoneGameObj *obj, PhysicalGameObj *Exiter);
	virtual void Object_Created_Event(GameObject* obj);
	virtual void Object_Destroyed_Event(GameObject* obj);
};

struct lua_State;

class LuaTT_DACrateClass : public DACrateClass {
public:
	static int Lua_Register_Crate(lua_State *L);

	LuaTT_DACrateClass::LuaTT_DACrateClass(lua_State *L_param, int uRef_param);
	virtual void Activate(cPlayer *Player);
	virtual void Init();
	virtual bool Can_Activate(cPlayer *Player);
private:
	lua_State *L = nullptr;
	int uRef = 0;
};

class LuaTT_DAConsoleFunction : public ConsoleFunctionClass {
public:
	static int Lua_Register_Console_Function(lua_State *L);
	LuaTT_DAConsoleFunction::LuaTT_DAConsoleFunction(lua_State *L_param, int uRef_param, const char *name, const char *alias, const char *help);

	const char *Get_Name() { return m_name; }
	const char *Get_Alias() { return m_alias; }
	const char *Get_Help() { return m_help; }
	void Activate(const char *ArgumentsString);

private:
	lua_State *L = nullptr;
	int uRef = 0;
	const char *m_name;
	const char *m_alias;
	const char *m_help;
};

#endif

//using std::string;


typedef void (*_SerialHook)(int, const char *);
typedef void (*_LoadingEHook)(int, bool);
typedef bool (*_DamageHook)(int, int, int, float, unsigned int);
typedef bool (*_ChatEHook)(int, int, char &, int);
typedef void (*_PingHook)(int, int);
typedef bool (*_SuicideHook)(int);
typedef bool (*_RadioHook)(int, int, int, int, int);

typedef void (*_AddSerialHook)(_SerialHook);
typedef void (*_AddLoadingEHook)(_LoadingEHook);
typedef void (*_AddDamageHook)(_DamageHook);
typedef void (*_AddChatEHook)(_ChatEHook);
typedef void (*_AddPingHook)(_PingHook);
typedef void (*_AddSuicideHook)(_SuicideHook);
typedef void (*_AddRadioHook)(_RadioHook);

typedef void (*_RequestSerial)(int, char &);

extern _RequestSerial RequestSerial;

extern _AddSerialHook AddSerialHook;
extern _AddLoadingEHook AddLoadingEHook;
extern _AddDamageHook AddDamageHook;
extern _AddChatEHook AddChatEHook;
extern _AddPingHook AddPingHook;
extern _AddSuicideHook AddSuicideHook;


#endif