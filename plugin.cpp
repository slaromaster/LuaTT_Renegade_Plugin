#include "General.h"
#include "engine_tt.h"
#include "engine_io.h"

#ifndef DAPLUGIN
#include "gmgame.h"
#endif

#include <stdarg.h>
#include "plugin.h"

extern "C" 
{
	#include "LuaLib\lua.h"
	#include "LuaLib\lualib.h"
	#include "LuaLib\lauxlib.h"
}

#include "LuaManager.h"

void LuaLoad()
{
	
	LuaManager::Load();
	LuaScriptManager::Load();
#ifdef LUATT_EXPORTS
	LuaManager::ShowBanner();
	LuaManager::LoadLuaPlugins();
#endif

}

