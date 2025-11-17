#pragma once

#ifdef DAPLUGIN
#include "da_event.h" //For DAEventClass definition.
#include "da_ssgm.h"
class LuaTT_DA_Hooks;
// These are missing exports, should be removed and replaced with DA TODO?
SCRIPTS_API void RegisterEvent(EventType Type, Plugin *Plugin);
SCRIPTS_API void UnregisterEvent(EventType Type, Plugin *Plugin);
#else
#include "gmplugin.h"
#endif
#include "ConnectionAcceptanceFilter.h"

//#define DEBUG_LUATT

class cGameData;
class StringClass;


class luatt :
	public Plugin,
	public ConnectionAcceptanceFilter
{
	void evict(int clientId, const WideStringClass& reason);
#ifdef DAPLUGIN
	LuaTT_DA_Hooks *daHooks;
#endif
public:
	
	luatt();
	~luatt();
	//virtual void OnLoadGlobalINISettings(INIClass *SSGMIni);
	//virtual void OnFreeData();
	//virtual void OnLoadMapINISettings(INIClass *SSGMIni);
	//virtual void OnFreeMapData();
	virtual bool OnChat(int PlayerID,TextMessageEnum Type,const wchar_t *Message,int recieverID);
	virtual void OnObjectCreate(void *data,GameObject *obj);
	virtual void OnLoadLevel();
	virtual void OnGameOver();
	virtual void OnPlayerJoin(int PlayerID,const char *PlayerName);
	virtual void OnPlayerLeave(int PlayerID);
	virtual bool OnRefill(GameObject *purchaser);
	virtual PurchaseStatus OnPowerupPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
	virtual PurchaseStatus OnVehiclePurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
	virtual PurchaseStatus OnCharacterPurchase(BaseControllerClass *base,GameObject *purchaser,unsigned int cost,unsigned int preset,const char *data);
	virtual void OnThink();
	virtual void OnDialog(int PlayerID, int DialogID, int ControlID, DialogMessageType MessageType); // override
	//virtual void Log_Output(const char *message);

	
	virtual void handleInitiation(const ConnectionRequest& connectionRequest);
	virtual void handleTermination(const ConnectionRequest& connectionRequest);
	virtual void handleCancellation(const ConnectionRequest& connectionRequest);
	STATUS getStatus(const ConnectionRequest& connectionRequest, WideStringClass& refusalMessage);
};


//extern luatt Luatt;