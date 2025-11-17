/*	Main loop glue hook
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2022 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#include "General.h"
#include "luatt.h"
#include "engine_tt.h"
#include "engine_io.h"
#include "LuaManager.h"
#include "LuaFunctions.h"

void __declspec(naked) main_loop_glue()
{
	__asm
	{
		call LuaManager::Call_Think_Hook;

		pop edi;
		pop esi;
		add esp, 0Ch;
		ret;
	}

}


