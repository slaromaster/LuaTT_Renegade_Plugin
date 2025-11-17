/*	Lua Renegade objects wrapper
Wraps Renegade structs to Lua
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2022 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#include "General.h"
#include "luatt.h"
#include "Lua_Class.h"

#define method(class, name) {#name, class::name}

const luaL_Reg LuaVector3::methods[] = {
  method(LuaVector3, AssignX),
  method(LuaVector3,AssignY),
  method(LuaVector3, AssignZ),
  method(LuaVector3, GetX),
  method(LuaVector3, GetY),
  method(LuaVector3, GetZ),
   
  method(LuaVector3, X),
  method(LuaVector3, Y),
  method(LuaVector3, Z),
  {0,0}
};

const luaL_Reg LuaBox::methods[] = {
  method(LuaBox, GetCenter),
  method(LuaBox, SetCenter),
  method(LuaBox, GetExtent),
  method(LuaBox, SetExtent),
  {0,0}
};

const luaL_Reg LuaMatrix3::methods[] = {
	method(LuaMatrix3, Get_X_Rotation),
	method(LuaMatrix3, Get_Y_Rotation),
	method(LuaMatrix3, Get_Z_Rotation),
	method(LuaMatrix3, Determinant),
	method(LuaMatrix3, Get_X_Vector),
	method(LuaMatrix3, Get_Y_Vector),
	method(LuaMatrix3, Get_Z_Vector),
	method(LuaMatrix3, Set),
	method(LuaMatrix3, Add),
	method(LuaMatrix3, Rotate_X),
	method(LuaMatrix3, Rotate_Y),
	method(LuaMatrix3, Rotate_Z),
	{ 0, 0 }
};


/*
const luaL_reg LuaSLNodeWrapper::methods[] = {
  method(LuaSLNodeWrapper, Next),
  method(LuaSLNodeWrapper, End),
  {0,0}
};
*/


const luaL_Reg LuaActionParams::methods[] = {
	/* function methods */
	method(LuaActionParams, Set_Movement),
	method(LuaActionParams, Set_Basic),

	/* setters / getters methods */
	method(LuaActionParams, AIState),
	method(LuaActionParams, Priority),
	method(LuaActionParams, ActionID),
	method(LuaActionParams, ObserverID),
	method(LuaActionParams, LookDuration),
	method(LuaActionParams, MoveSpeed),
	method(LuaActionParams, MoveBackup),
	method(LuaActionParams, MoveFollow),
	method(LuaActionParams, MoveCrouched),
	method(LuaActionParams, MovePathfind),
	method(LuaActionParams, ShutdownEngineOnArrival),
	method(LuaActionParams, AttackRange),
	method(LuaActionParams, AttackError),
	method(LuaActionParams, AttackErrorOverride),
	method(LuaActionParams, AttackPrimaryFire),
	method(LuaActionParams, AttackCrouched),
	method(LuaActionParams, AttackCheckBlocked),
	method(LuaActionParams, AttackActive),
	method(LuaActionParams, AttackWanderAllowed),
	method(LuaActionParams, AttackFaceTarget),
	method(LuaActionParams, AttackForceFire),
	method(LuaActionParams, ForceFacing),
	method(LuaActionParams, FaceDuration),
	method(LuaActionParams, IgnoreFacing),
	method(LuaActionParams, WaypathID),
	method(LuaActionParams, WaypointStartID),
	method(LuaActionParams, WaypointEndID),
	method(LuaActionParams, WaypathSplined),
	method(LuaActionParams, AnimationLooping),
	method(LuaActionParams, ActiveConversationID),
	method(LuaActionParams, MoveArrivedDistance),
	method(LuaActionParams, DockLocation),
	method(LuaActionParams, DockEntrance),
	method(LuaActionParams, MoveLocation),
	method(LuaActionParams, MoveObjectOffset),
	method(LuaActionParams, AttackLocation),
	method(LuaActionParams, FaceLocation),
	method(LuaActionParams, LookLocation),
	method(LuaActionParams, MoveObject),
	method(LuaActionParams, LookObject),
	method(LuaActionParams, AttackObject),
	method(LuaActionParams, ConversationName),
	method(LuaActionParams, AnimationName),

	{ 0, 0 }
};
