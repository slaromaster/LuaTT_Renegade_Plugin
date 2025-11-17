#ifndef LUATT_INCLUDE__GENERAL_H
#define LUATT_INCLUDE__GENERAL_H

#define LUATT_BUILD "170224"

#include "Defines.h"
#include "Standard.h"

#define lua_boxpointer(L,u) \
	(*(void **)(lua_newuserdata(L, sizeof(void *))) = (u))
#define lua_unboxpointer(L,i)	(*(void **)(lua_touserdata(L, i)))

#endif


