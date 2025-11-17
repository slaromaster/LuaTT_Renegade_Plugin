/*	Header of Lua Class
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2022 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#ifndef INCLUDE_CLASS
#define INCLUDE_CLASS

#include "LuaLib\lua.hpp"
#include "LuaManager.h"
#pragma warning(disable: 4291)

class LUA_Vector3 
{
private:
    float X_;
	float Y_;
	float Z_;
  public:
	LUA_Vector3()
	{
	}
    LUA_Vector3(float x, float y, float z)
	{
	  X_ = x;
	  Y_ = y;
	  Z_ = z;
	}
	float GetX()
	{
	  return X_;
	}
	float GetY()
	{
	  return Y_;
	}
	float GetZ()
	{
	  return Z_;
	}
	
	void AssignX(float x)
	{
	  X_ = x;
	}
	void AssignY(float y)
	{
	  Y_ = y;
	}
	void AssignZ(float z)
	{
	  Z_ = z;
	}
  

	float X()
	{
	  return X_;
	}
	float Y()
	{
	  return Y_;
	}
	float Z()
	{
	  return Z_;
	}

	void X(float x)
	{
	  X_ = x;
	}
	void Y(float y)
	{
	  Y_ = y;
	}
	void Z(float z)
	{
	  Z_ = z;
	}
};

class LuaVector3
{
	static const luaL_Reg methods[];
  

  static int RegisterTable(lua_State *L) 
  {
	float x = (float)luaL_checknumber(L, 1);
	float  y = (float)luaL_checknumber(L, 2);
	float z = (float)luaL_checknumber(L, 3);
	LUA_Vector3 *a = new LUA_Vector3(x, y, z);
	lua_boxpointer(L, a);
	luaL_getmetatable(L, "Vector3");
	lua_setmetatable(L, -2);
	return 1;
  }
  static int GetX(lua_State *L)
  {
	LUA_Vector3 *a = GetInstance(L, 1);
	lua_pushnumber(L, a->GetX());
	return 1;
  }
  static int GetY(lua_State *L)
  {
    LUA_Vector3 *a = GetInstance(L, 1);
	lua_pushnumber(L, a->GetY());
	return 1;
  }
  static int GetZ(lua_State *L)
  {
	LUA_Vector3 *a = GetInstance(L, 1);
	lua_pushnumber(L, a->GetZ());
	return 1;
  }

  static int AssignX(lua_State *L)
  {
	LUA_Vector3 *a = GetInstance(L, 1);
	a->AssignX((float)luaL_checknumber(L, 2));
	return 0;
  }
  static int AssignY(lua_State *L)
  {
    LUA_Vector3 *a = GetInstance(L, 1);
	a->AssignY((float)luaL_checknumber(L, 2));
	return 0;
  }
  static int AssignZ(lua_State *L)
  {
	LUA_Vector3 *a = GetInstance(L, 1);
	a->AssignZ((float)luaL_checknumber(L, 2));
	return 0;
  }



	static int X(lua_State *L)
	{
		int argc = lua_gettop(L);
		if(argc < 1)
		{
			LUA_Vector3 *a = GetInstance(L, 1);
			lua_pushnumber(L, a->GetX());
			return 1;
		}
		else
		{
			LUA_Vector3 *a = GetInstance(L, 1);
			a->AssignX((float)luaL_checknumber(L, 2));
			return 0;
		}
	}
	static int Y(lua_State *L)
	{
		int argc = lua_gettop(L);
		if(argc < 1)
		{
			LUA_Vector3 *a = GetInstance(L, 1);
			lua_pushnumber(L, a->GetY());
			return 1;
		}
		else
		{
			LUA_Vector3 *a = GetInstance(L, 1);
			a->AssignY((float)luaL_checknumber(L, 2));
			return 0;
		}
	}
	static int Z(lua_State *L)
	{
		int argc = lua_gettop(L);
		if(argc < 1)
		{
			LUA_Vector3 *a = GetInstance(L, 1);
			lua_pushnumber(L, a->GetZ());
			return 1;
		}
		else
		{
			LUA_Vector3 *a = GetInstance(L, 1);
			a->AssignZ((float)luaL_checknumber(L, 2));
			return 0;
		}
	}
	static int DeleteInstance(lua_State *L) 
	{
		 LUA_Vector3  *a = (LUA_Vector3 *)lua_unboxpointer(L, 1);
		  delete a;
		  return 0;
	}
public:
	static void Register(lua_State *L)
	{
		lua_newtable(L);                
		int methodtable = lua_gettop(L);
		luaL_newmetatable(L, "Vector3"); 
		int metatable = lua_gettop(L);

		lua_pushliteral(L, "__metatable");
		lua_pushvalue(L, methodtable);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, methodtable);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, DeleteInstance);
		lua_settable(L, metatable);

		lua_pop(L, 1);

		luaL_openlib(L, 0, methods, 0);
		lua_pop(L, 1);  

		lua_register(L, "Vector3", RegisterTable);
	}
	static LUA_Vector3 *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		void *ud = luaL_checkudata(L, n, "Vector3");
		if(!ud) 
		{
			luaL_typerror(L, n, "Vector3");
		}
		return *(LUA_Vector3**)ud;
	}
};

class LuaMatrix3
{
	static const luaL_Reg methods[];


	static int RegisterTable(lua_State *L)
	{

		Matrix3 *a;
		int argc = lua_gettop(L);
		if (argc == 0)
		{
			a = new Matrix3();
		}
		else if (argc == 1)
		{
			a = new Matrix3(static_cast<bool>(lua_toboolean(L, 1)));
		}
		else if (argc == 2)
		{
			LUA_Vector3 *v = LuaVector3::GetInstance(L, 1);
			Vector3 axis = Vector3(v->X(), v->Y(), v->Z()); 
			a = new Matrix3(axis, lua_tonumber(L, 2));
		}
		else if (argc == 3)
		{
			LUA_Vector3 *a1 = LuaVector3::GetInstance(L, 1);
			LUA_Vector3 *a2 = LuaVector3::GetInstance(L, 2);
			LUA_Vector3 *a3 = LuaVector3::GetInstance(L, 3);
			Vector3 v1 = Vector3(a1->X(), a1->Y(), a1->Z());
			Vector3 v2 = Vector3(a2->X(), a2->Y(), a2->Z());
			Vector3 v3 = Vector3(a3->X(), a3->Y(), a3->Z());
			a = new Matrix3(v1, v2, v3);
		}
		else
		{
			return 0;
		}

		lua_boxpointer(L, a);
		luaL_getmetatable(L, "Matrix3");
		lua_setmetatable(L, -2);
		return 1;
	}

	static int Get_X_Rotation(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		lua_pushnumber(L, a->Get_X_Rotation());
		return 1;
	}
	
	static int Get_Y_Rotation(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		lua_pushnumber(L, a->Get_Y_Rotation());
		return 1;
	}

	static int Get_Z_Rotation(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		lua_pushnumber(L, a->Get_Z_Rotation());
		return 1;
	}

	static int Determinant(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		lua_pushnumber(L, a->Determinant());
		return 1;
	}

	static int Get_X_Vector(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		Vector3 pos = a->Get_X_Vector();
		LUA_Vector3 *vec = new LUA_Vector3(pos.X, pos.Y, pos.Z);
		lua_boxpointer(L, vec);
		luaL_getmetatable(L, "Vector3");
		lua_setmetatable(L, -2);
		return 1;
	}

	static int Get_Y_Vector(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		Vector3 pos = a->Get_Y_Vector();
		LUA_Vector3 *vec = new LUA_Vector3(pos.X, pos.Y, pos.Z);
		lua_boxpointer(L, vec);
		luaL_getmetatable(L, "Vector3");
		lua_setmetatable(L, -2);
		return 1;
	}

	static int Get_Z_Vector(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		Vector3 pos = a->Get_Z_Vector();
		LUA_Vector3 *vec = new LUA_Vector3(pos.X, pos.Y, pos.Z);
		lua_boxpointer(L, vec);
		luaL_getmetatable(L, "Vector3");
		lua_setmetatable(L, -2);
		return 1;
	}

	static int Set(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		int argc = lua_gettop(L);
		if (argc == 3)
		{
			LUA_Vector3 *a1 = LuaVector3::GetInstance(L, 2);
			LUA_Vector3 *a2 = LuaVector3::GetInstance(L, 3);
			LUA_Vector3 *a3 = LuaVector3::GetInstance(L, 4);
			Vector3 v1 = Vector3(a1->X(), a1->Y(), a1->Z());
			Vector3 v2 = Vector3(a2->X(), a2->Y(), a2->Z());
			Vector3 v3 = Vector3(a3->X(), a3->Y(), a3->Z());
			a->Set(v1, v2, v3);
		}
		else if (argc == 9)
		{
			a->Set(	lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4),
					lua_tonumber(L, 5), lua_tonumber(L, 6), lua_tonumber(L, 7),
					lua_tonumber(L, 8), lua_tonumber(L, 9), lua_tonumber(L, 10));
		}
		
		return 1;
	}

	static int Add(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		Matrix3 *b = GetInstance(L, 2);
		a->Add(*a, *b, a);
		return 1;
	}

	static int Rotate_X(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		a->Rotate_X(lua_tonumber(L, 2));
		return 1;
	}

	static int Rotate_Y(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		a->Rotate_X(lua_tonumber(L, 2));
		return 1;
	}

	static int Rotate_Z(lua_State *L)
	{
		Matrix3 *a = GetInstance(L, 1);
		a->Rotate_X(lua_tonumber(L, 2));
		return 1;
	}


	static int DeleteInstance(lua_State *L)
	{
		Matrix3  *a = (Matrix3 *)lua_unboxpointer(L, 1);
		delete a;
		return 0;
	}
public:
	static void Register(lua_State *L)
	{
		lua_newtable(L);
		int methodtable = lua_gettop(L);
		luaL_newmetatable(L, "Matrix3");
		int metatable = lua_gettop(L);

		lua_pushliteral(L, "__metatable");
		lua_pushvalue(L, methodtable);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, methodtable);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, DeleteInstance);
		lua_settable(L, metatable);

		lua_pop(L, 1);

		luaL_openlib(L, 0, methods, 0);
		lua_pop(L, 1);

		lua_register(L, "Matrix3", RegisterTable);
	}
	static Matrix3 *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		void *ud = luaL_checkudata(L, n, "Matrix3");
		if (!ud)
		{
			luaL_typerror(L, n, "Matrix3");
		}
		return *(Matrix3**)ud;
	}
};

class Box
{
public:
	OBBoxClass box;
	Box()
	{
		box.Center = Vector3(0.0, 0.0, 0.0);
		box.Extent = Vector3(0.0, 0.0, 0.0);
		for(int i = 0; i < 3; i++)
		{
			for(int y = 0; y < 3; y++)
			{
				box.Basis[i][y] = 0.0;
			}
		}
		
	}
    Box(float X, float Y, float Z, float Width, float Length, float Height)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int y = 0; y < 3; y++)
			{
				box.Basis[i][y] = 0.0;
			}
		}
		box.Center = Vector3(X, Y, Z);
		box.Extent = Vector3(Width, Length, Height);
	}
	Box(Vector3 pos, Vector3 size, Matrix3 rot)
	{
		box = OBBoxClass(pos, size, rot);
	}
};

class LuaBox
{
	static const luaL_Reg methods[];
  

	static int RegisterTable(lua_State *L) 
	{
		int argc = lua_gettop(L);
		if(argc < 3)
		{
			Box *a = new Box();
			lua_boxpointer(L, a);
			luaL_getmetatable(L, "Box");
			lua_setmetatable(L, -2);
			return 1;
		}
		else if (argc == 3)
		{
			LUA_Vector3 *a1 = LuaVector3::GetInstance(L, 1);
			LUA_Vector3 *a2 = LuaVector3::GetInstance(L, 2);
			Matrix3 *mat = LuaMatrix3::GetInstance(L, 3);
			Vector3 position = Vector3(a1->X(), a1->Y(), a1->Z());
			Vector3 size = Vector3(a2->X(), a2->Y(), a2->Z());
			Box *a = new Box(position, size, *mat);
			lua_boxpointer(L, a);
			luaL_getmetatable(L, "Box");
			lua_setmetatable(L, -2);
			return 1;
		}
		else if (argc == 6)
		{
			Box *a = new Box((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4), (float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6));
			lua_boxpointer(L, a);
			luaL_getmetatable(L, "Box");
			lua_setmetatable(L, -2);
			return 1;
		}
		
		return 0;
	}
	static int GetCenter(lua_State *L)
	{
		Box *b = GetInstance(L, 1);

		LUA_Vector3 *a = new LUA_Vector3(b->box.Center.X, b->box.Center.Y, b->box.Center.Z);
		lua_boxpointer(L, a);
		luaL_getmetatable(L, "Vector3");
		lua_setmetatable(L, -2);
		return 1;
	}

	static int SetCenter(lua_State *L)
	{
		int argc = lua_gettop(L);
		if(argc < 3)
		{
			return 0;
		}
		Box *b = GetInstance(L, 1);
		b->box.Center = Vector3((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		return 0;
	}
	static int GetExtent(lua_State *L)
	{
		Box *b = GetInstance(L, 1);

		LUA_Vector3 *a = new LUA_Vector3((float)b->box.Extent.X, (float)b->box.Extent.Y, (float)b->box.Extent.Z);
		lua_boxpointer(L, a);
		luaL_getmetatable(L, "Vector3");
		lua_setmetatable(L, -2);
		return 1;
	}

	static int SetExtent(lua_State *L)
	{
		int argc = lua_gettop(L);
		if(argc < 3)
		{
			return 0;
		}
		Box *b = GetInstance(L, 1);
		b->box.Extent = Vector3((float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));
		return 0;
	}
	static int DeleteInstance(lua_State *L) 
	{
		Box  *a = (Box *)lua_unboxpointer(L, 1);
		delete a;
		return 0;
	}
public:
  static void Register(lua_State *L)
  {
	lua_newtable(L);                
	int methodtable = lua_gettop(L);
	luaL_newmetatable(L, "Box"); 
	int metatable = lua_gettop(L);

	lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, methodtable);
    lua_settable(L, metatable);

    lua_pushliteral(L, "__index");
    lua_pushvalue(L, methodtable);
    lua_settable(L, metatable);

	lua_pushliteral(L, "__gc");
    lua_pushcfunction(L, DeleteInstance);
    lua_settable(L, metatable);

	lua_pop(L, 1);

	luaL_openlib(L, 0, methods, 0);
    lua_pop(L, 1);  

    lua_register(L, "Box", RegisterTable);
  }
	static Box *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		void *ud = luaL_checkudata(L, n, "Box");
		if(!ud) luaL_typerror(L, n, "Box");
		return *(Box**)ud;
	}
};

/*
class SLNodeWrapper
{
	int *Data;


public:
	int _Type;
    SLNodeWrapper(int Type)
	{
		GenericSLNode *Iterator = 0;
		_Type = Type;
		if(Type == 0 || (Type < 0 || Type > 3))
		{
			_Type = 0;
			Iterator = PlayerList->HeadNode;
		}
		else if(Type == 1)
		{
			Iterator = BuildingGameObjList->HeadNode;
		}
		else if(Type == 2)
		{
			Iterator = GameObjManager::SmartGameObjList.Head();
		}
		else if(Type == 3)
		{		
			Iterator = GameObjManager::SmartGameObjList
		}

		Data = (int *)malloc(4);
		*Data = 0;
		for(int x = 0; Iterator != 0; Iterator = Iterator->NodeNext, x++)
		{
			Data = (int *)realloc(Data, (x+2)*4);
			Data[x] = Type == 0 ? ((cPlayer *)Iterator->NodeData)->PlayerId : Commands->Get_ID((GameObject *)Iterator->NodeData);
			Data[x+1] = 0;
		}
	}

	int Next()
	{
		if(*Data != 0)
		{
			return *(Data++);
		}

		return -1;
	}

	int End()
	{
		if(*Data != 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
};

class LuaSLNodeWrapper
{
  static const luaL_reg methods[];
  

	static int RegisterTable(lua_State *L) 
	{
		int argc = lua_gettop(L);

		SLNodeWrapper *a = new SLNodeWrapper(argc < 1 ? 0 : (int)lua_tonumber(L, 1));
		lua_boxpointer(L, a);
		luaL_getmetatable(L, "SLNode");
		lua_setmetatable(L, -2);		
		return 1;
	}

	static int Next(lua_State *L)
	{
		SLNodeWrapper *a = GetInstance(L, 1);
		lua_pushnumber(L, a->Next());
		return 1;
	}
	static int End(lua_State *L)
	{
		SLNodeWrapper *a = GetInstance(L, 1);
		lua_pushnumber(L, a->End());
		return 1;
	}

	static int DeleteInstance(lua_State *L) 
	{
		SLNodeWrapper *a = (SLNodeWrapper *)lua_unboxpointer(L, 1);
		delete a;
		return 0;
	}
public:
	static void Register(lua_State *L)
	{
		lua_newtable(L);                
		int methodtable = lua_gettop(L);
		luaL_newmetatable(L, "SLNode"); 
		int metatable = lua_gettop(L);

		lua_pushliteral(L, "__metatable");
		lua_pushvalue(L, methodtable);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, methodtable);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, DeleteInstance);
		lua_settable(L, metatable);

		lua_pop(L, 1);

		luaL_openlib(L, 0, methods, 0);
		lua_pop(L, 1);  

		lua_register(L, "SLNode", RegisterTable);
	}
	static SLNodeWrapper *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		void *ud = luaL_checkudata(L, n, "SLNode");
		if(!ud) luaL_typerror(L, n, "SLNode");
		return *(SLNodeWrapper**)ud;
	}
};


*/

/* Macros used to wrap the variables of ActionParamsStruct */

#define LuaActionParams_NUMBER(name, type) static int name(lua_State *L) \
{ \
	if (lua_gettop(L) < 1) return 0; \
	ActionParamsStruct *a = GetInstance(L, 1); \
	if (!a) return 0; \
	if (lua_gettop(L) >= 2) \
	{ \
		a->name = static_cast<type>(lua_tonumber(L, 2)); \
	} \
	else \
	{ \
		lua_pushnumber(L, a->name); \
	} \
	return 1; \
}

#define LuaActionParams_INT(name) LuaActionParams_NUMBER(name, int)
#define LuaActionParams_FLOAT(name) LuaActionParams_NUMBER(name, float)
#define LuaActionParams_BOOL(name) LuaActionParams_NUMBER(name, bool)


#define LuaActionParams_VEC3(name) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	ActionParamsStruct *a = GetInstance(L, 1); \
	if (!a) return 0; \
	if (lua_gettop(L) >= 2) \
	{ \
		LUA_Vector3 *vc = LuaVector3::GetInstance(L, 2); \
		a->name = Vector3(vc->X(), vc->Y(), vc->Z()); \
	} \
	else \
	{ \
		LUA_Vector3 *vc = new LUA_Vector3(a->name.X, a->name.Y, a->name.Z); \
		lua_boxpointer(L, vc); \
		luaL_getmetatable(L, "Vector3"); \
		lua_setmetatable(L, -2); \
	} \
	return 1; \
}

#define LuaActionParams_GO(name) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	ActionParamsStruct *a = GetInstance(L, 1); \
if (!a) return 0; \
if (lua_gettop(L) >= 2) \
	{ \
	GameObject *vc = Commands->Find_Object(static_cast<int>(lua_tonumber(L, 2))); \
	if (!vc) return 0; \
	a->name = vc; \
	} \
	else \
	{ \
	GameObject *vc = a->name; \
	lua_pushnumber(L, Commands->Get_ID(vc)); \
	} \
	return 1; \
}

#define LuaActionParams_CHAR(name) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	ActionParamsStruct *a = GetInstance(L, 1); \
if (!a) return 0; \
if (lua_gettop(L) >= 2) \
	{ \
	a->name = lua_tostring(L, 2); \
	} \
	else \
	{ \
	lua_pushstring(L, a->name); \
	} \
	return 1; \
}

/* END Macros used to wrap the variables of ActionParamsStruct */

class LuaActionParams
{
	static const luaL_Reg methods[];


	static int RegisterTable(lua_State *L)
	{
		ActionParamsStruct *a = new ActionParamsStruct();
		lua_boxpointer(L, a);
		luaL_getmetatable(L, "ActionParamsStruct");
		lua_setmetatable(L, -2);
		return 1;
	}

	LuaActionParams_FLOAT(MoveArrivedDistance);
	LuaActionParams_INT(Priority);
	LuaActionParams_INT(ActionID);
	LuaActionParams_INT(ObserverID);
	LuaActionParams_FLOAT(LookDuration);
	LuaActionParams_FLOAT(MoveSpeed);
	LuaActionParams_BOOL(MoveBackup);
	LuaActionParams_BOOL(MoveFollow);
	LuaActionParams_BOOL(MoveCrouched);
	LuaActionParams_BOOL(MovePathfind);
	LuaActionParams_INT(ShutdownEngineOnArrival);
	LuaActionParams_FLOAT(AttackRange);
	LuaActionParams_FLOAT(AttackError);
	LuaActionParams_INT(AttackErrorOverride);
	LuaActionParams_INT(AttackPrimaryFire);
	LuaActionParams_INT(AttackCrouched);
	LuaActionParams_INT(AttackCheckBlocked);
	LuaActionParams_INT(AttackActive);
	LuaActionParams_INT(AttackWanderAllowed);
	LuaActionParams_INT(AttackFaceTarget);
	LuaActionParams_INT(AttackForceFire);
	LuaActionParams_INT(ForceFacing);
	LuaActionParams_FLOAT(FaceDuration);
	LuaActionParams_INT(IgnoreFacing);
	LuaActionParams_INT(WaypathID);
	LuaActionParams_INT(WaypointStartID);
	LuaActionParams_INT(WaypointEndID);
	LuaActionParams_INT(WaypathSplined);
	LuaActionParams_INT(AnimationLooping);
	LuaActionParams_INT(ActiveConversationID);


	LuaActionParams_VEC3(DockLocation);
	LuaActionParams_VEC3(DockEntrance);
	LuaActionParams_VEC3(MoveLocation);
	LuaActionParams_VEC3(MoveObjectOffset);
	LuaActionParams_VEC3(AttackLocation);
	LuaActionParams_VEC3(FaceLocation);
	LuaActionParams_VEC3(LookLocation);

	LuaActionParams_GO(MoveObject);
	LuaActionParams_GO(LookObject);
	LuaActionParams_GO(AttackObject);
	LuaActionParams_CHAR(ConversationName);
	LuaActionParams_CHAR(AnimationName);


	/*
	void Set_Basic_Old(ScriptClass *s,unsigned long priority,unsigned long ID,long state);
	void Set_Attack_Hold(GameObject *Target,float range,float deviation,bool primary,bool Hold);
	void Set_Attack_Position(const Vector3 & position,float Range,float Deviation,bool Primary);
	void Set_Goto_Location(const Vector3 & position,float speed,float arrivedistance,bool crouch);
	void Set_Goto_Following(GameObject *Target,float speed,float arrivedistance,bool following);
	void Set_Goto_Crouch(GameObject *Target,float speed,float arrivedistance,bool crouch);
	void Set_Goto_Crouch_Following(GameObject *Target,float speed,float arrivedistance,bool crouch,bool following);
	void Set_Goto(GameObject *Target,float speed,float arrivedistance);
	void Set_Goto_Waypath_Start(unsigned long waypathID,unsigned long waypathstartID,unsigned long waypathendID,bool splined);
	void Set_Goto_Waypath(unsigned long waypathID,bool splined,bool v57,bool Hold);
	void Set_Goto_Waypath_ID(unsigned long waypathID);
	void Set_Move_Arrive_Distance(float distance);
	*void Set_Basic( GameObjObserverClass * script, float priority, int action_id, SoldierAIState ai_state = NO_AI_STATE_CHANGE ) { ObserverID = script->Get_ID(); Priority = (int)priority; ActionID = action_id; AIState = ai_state; }
	void Set_Basic( long observer_id, float priority, int action_id, SoldierAIState ai_state = NO_AI_STATE_CHANGE ) { ObserverID = observer_id; Priority = (int)priority; ActionID = action_id; AIState = ai_state; }
	void Set_Look( const Vector3 & location, float duration ) { LookLocation = location; LookDuration = duration; }	
	void Set_Look( GameObject * object, float duration ) { LookObject = object; LookDuration = duration; }
	void Set_Look( const Vector3 & obj_pos, float angle,  float duration );
	*void Set_Movement( const Vector3 & location, float speed, float arrived_distance, bool crouched = false ) { MoveLocation = location; MoveSpeed = speed; MoveArrivedDistance = arrived_distance; MoveCrouched = crouched; }
	void Set_Movement( GameObject *	object, float speed, float arrived_distance, bool crouched = false ) { MoveObject = object; MoveSpeed = speed; MoveArrivedDistance = arrived_distance; MoveCrouched = crouched; }
	void Set_Attack( const Vector3 & location, float range, float error, bool primary_fire ) { AttackLocation = location; AttackRange = range; AttackError = error; AttackPrimaryFire = primary_fire; }
	void Set_Attack( GameObject * object, float range, float error, bool primary_fire ) { AttackObject = object; AttackRange = range; AttackError = error; AttackPrimaryFire = primary_fire; }
	void Set_Animation( const char * name, bool looping ) { AnimationName = name; AnimationLooping = looping; }
	void Set_Face_Location( const Vector3 &location, float duration )	{ FaceLocation = location; FaceDuration = duration; }
	void Set_Face_Location(  const Vector3 & obj_pos, float angle, float duration );
	void Join_Conversation( int active_conversation_id )	{ ActiveConversationID = active_conversation_id; }
	void Start_Conversation( const char * name )			{ ConversationName = name; }
	void Dock_Vehicle( const Vector3 &dock_location, const Vector3 &dock_entrance ) { DockLocation = dock_location; DockEntrance = dock_entrance; }

	*/

	static int AIState(lua_State *L) 
	{ 
		if (lua_gettop(L) < 1) return 0; 
		ActionParamsStruct *a = GetInstance(L, 1); 
		if (!a) return 0; 
		if (lua_gettop(L) >= 2) 
		{ 
			a->AIState = (SoldierAIState)(int)lua_tonumber(L, 2); 
		} 
		else 
		{ 
			lua_pushnumber(L, a->AIState); 
		} 
		return 1; 
	}


	static int Set_Basic(lua_State *L)
	{
		if (lua_gettop(L) < 5) return 0;
		ActionParamsStruct *a = GetInstance(L, 1);
		if (!a) return 0;
		GameObject *obj = Commands->Find_Object(static_cast<int>(lua_tonumber(L, 3)));
		GameObjObserverClass * script = LuaScriptManager::Get_Script(obj, static_cast<int>(lua_tonumber(L, 2)));
		float priority = static_cast<float>(lua_tonumber(L, 4));
		int action_id = static_cast<int>(lua_tonumber(L, 5));
		SoldierAIState ai_state = NO_AI_STATE_CHANGE;

		if (lua_gettop(L) >= 6)
		{
			ai_state = (SoldierAIState)(int)lua_tonumber(L, 6);
		}

		a->Set_Basic(script, priority, action_id, ai_state);
		return 1;
	}

	static int Set_Movement(lua_State *L)
	{
		if (lua_gettop(L) < 4) return 0;
		ActionParamsStruct *a = GetInstance(L, 1);
		if (!a) return 0;
		bool useObj = false;
		LUA_Vector3 *vec = nullptr;
		GameObject *obj;
		if (lua_isuserdata(L, 2) == 1)
		{
			vec = LuaVector3::GetInstance(L, 2);
		}
		else
		{
			useObj = true;
		}


		float speed = lua_tonumber(L, 3);
		float arrived_distance = lua_tonumber(L, 4);
		bool crouched = false;

		if (lua_gettop(L) >= 5)
		{
			crouched = lua_toboolean(L, 5);
		}
		if (useObj)
		{
			obj = Commands->Find_Object(static_cast<int>(lua_tonumber(L, 2)));
			a->Set_Movement(obj, speed, arrived_distance, crouched);
		}
		else if (vec != nullptr)
		{
			Vector3 location = Vector3(vec->X(), vec->Y(), vec->Z());
			a->Set_Movement(location, speed, arrived_distance, crouched);
		}
		return 1;
	}
		
	
	static int DeleteInstance(lua_State *L)
	{
		ActionParamsStruct  *a = (ActionParamsStruct *)lua_unboxpointer(L, 1);
		delete a;
		return 0;
	}
public:
	static void Register(lua_State *L)
	{
		lua_newtable(L);
		int methodtable = lua_gettop(L);
		luaL_newmetatable(L, "ActionParamsStruct");
		int metatable = lua_gettop(L);

		lua_pushliteral(L, "__metatable");
		lua_pushvalue(L, methodtable);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, methodtable);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, DeleteInstance);
		lua_settable(L, metatable);

		lua_pop(L, 1);

		luaL_openlib(L, 0, methods, 0);
		lua_pop(L, 1);

		lua_register(L, "ActionParamsStruct", RegisterTable);
	}
	static ActionParamsStruct *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		void *ud = luaL_checkudata(L, n, "ActionParamsStruct");
		if (!ud)
		{
			luaL_typerror(L, n, "ActionParamsStruct");
		}
		return *(ActionParamsStruct**)ud;
	}
};

#endif