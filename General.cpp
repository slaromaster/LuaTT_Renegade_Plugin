#include "general.h"

void *lua_testudata(lua_State *L, int idx, const char *tname)
{
	void *p = lua_touserdata(L, idx);
	if (p == NULL) return NULL;

	if (lua_getmetatable(L, idx)) 
	{
		luaL_getmetatable(L, tname);
		int equal = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (equal) return p;
	}

	return NULL;
}
