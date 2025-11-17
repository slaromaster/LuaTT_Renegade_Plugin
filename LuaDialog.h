/*	Lua Dialog header
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2022 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#ifndef INCLUDE_DIALOG
#define INCLUDE_DIALOG

#include "LuaLib\lua.hpp"
#include "ScriptedControls.h"

#define LuaScriptedControlClass_Set(name, subclass, type) static int name(lua_State *L) \
{ \
	if (lua_gettop(L) < 2) return 0; \
	ScriptedControlClass *a = GetInstance(L, 1); \
	if (a != nullptr) \
	{ \
	    auto sub = a->As_##subclass(); \
		if (sub != nullptr) \
		{ \
		    auto num = lua_to##type(L, 2); \
			sub->name(num); \
		} \
	} \
	return 0; \
}

#define LuaScriptedControlClass_Get(name, subclass, type) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	ScriptedControlClass *a = GetInstance(L, 1); \
if (a != nullptr) \
	{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
		{ \
		auto num = sub->name(); \
		lua_push##type(L, num); \
		return 1; \
		} \
	} \
	return 0; \
}

#define LuaScriptedControlClass_Set2(name, subclass, subclass2, type) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 2) return 0; \
	ScriptedControlClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
	if (sub != nullptr) \
	{ \
		auto num = lua_to##type(L, 2); \
		sub->name(num); \
	} \
	else \
	{ \
		auto sub2 = a->As_##subclass2(); \
		if (sub2 != nullptr) \
		{ \
			auto num = lua_to##type(L, 2); \
			sub2->name(num); \
		} \
	} \
} \
	return 0; \
}

#define LuaScriptedControlClass_Get2(name, subclass, subclass2, type) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	ScriptedControlClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
	if (sub != nullptr) \
		{ \
			auto num = sub->name(); \
			lua_push##type(L, num); \
			return 1; \
	} \
	else \
	{ \
			auto sub2 = a->As_##subclass2(); \
			if (sub2 != nullptr) \
			{ \
				auto num = sub2->name(); \
				lua_push##type(L, num); \
				return 1; \
			} \
	} \
} \
	return 0; \
}

#define LuaScriptedControlClass_Call(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	ScriptedControlClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
	if (sub != nullptr) \
	{ \
		sub->name(); \
	} \
} \
return 0; \
}

#define LuaScriptedControlClass_SetText(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 2) return 0; \
	ScriptedControlClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
	if (sub != nullptr) \
	{ \
		auto str = lua_tostring(L, 2); \
		WideStringClass wch(str); \
		sub->name(wch); \
	} \
} \
	return 0; \
}

#define LuaScriptedControlClass_GetText(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	ScriptedControlClass *a = GetInstance(L, 1); \
if (a != nullptr) \
	{ \
	auto sub = a->As_##subclass(); \
	if (sub != nullptr) \
		{ \
		auto wch = sub->name(); \
		StringClass str; \
		wch.Convert_To(str); \
		lua_pushstring(L, str.Peek_Buffer()); \
		return 1; \
		} \
	} \
	return 0; \
}

class LuaScriptedControlClass
{
	static const luaL_Reg methods[];

	/* ScriptedControlClass */
	static int Get_Control_ID(lua_State *L);
	static int Get_Location(lua_State *L);
	static int Get_Size(lua_State *L);
	static int Is_Enabled(lua_State *L);
	static int Is_Dirty(lua_State *L);
	static int Set_Location(lua_State *L);
	static int Set_Size(lua_State *L);
	static int Set_Enabled(lua_State *L);
	static int Set_Dirty(lua_State *L);

	/* ScriptedLabelControlClass  */
	static int Get_Label_Text(lua_State *L);
	static int Get_Text_Color(lua_State *L);
	static int Get_Style(lua_State *L);
	static int Get_Orientation(lua_State *L);
	static int Set_Label_Text(lua_State *L);
	static int Set_Text_Color(lua_State *L);
	static int Set_Style(lua_State *L);
	static int Set_Orientation(lua_State *L);
	
	/* ScriptedImageControlClass */
	static int Get_Image_Name(lua_State *L);
	static int Set_Image_Name(lua_State *L);

	/* ScriptedButtonControlClass */
	static int Get_Button_Text(lua_State *L);
	static int Get_Button_Up_Image_Name(lua_State *L);
	static int Get_Button_Down_Image_Name(lua_State *L);
	static int Get_Button_Style(lua_State *L);
	static int Set_Button_Down_Image_Name(lua_State *L);
	static int Set_Button_Style(lua_State *L);
	static int Set_Button_Up_Image_Name(lua_State *L);
	static int Set_Button_Text(lua_State *L);


	/* ScriptedTextAreaControlClass */
	LuaScriptedControlClass_Get(Get_Text_Limit, ScriptedTextAreaControlClass, number)
	LuaScriptedControlClass_Get(Is_Password_Field, ScriptedTextAreaControlClass, boolean)
	LuaScriptedControlClass_Get(Is_Numeric_Field, ScriptedTextAreaControlClass, boolean)
	LuaScriptedControlClass_Get(Is_Automatic_Horizontal_Scroll, ScriptedTextAreaControlClass, boolean)
	LuaScriptedControlClass_Get(Get_Text_Length, ScriptedTextAreaControlClass, number)

	LuaScriptedControlClass_Set(Set_Text_Limit, ScriptedTextAreaControlClass, integer)
	LuaScriptedControlClass_Set(Set_Password_Field, ScriptedTextAreaControlClass, boolean)
	LuaScriptedControlClass_Set(Set_Numeric_Field, ScriptedTextAreaControlClass, boolean)
	LuaScriptedControlClass_Set(Set_Automatic_Horizontal_Scroll, ScriptedTextAreaControlClass, boolean)
	static int Set_Text(lua_State *L);
	static int Get_Text(lua_State *L);

	/* ScriptedCheckBoxControlClass */
	LuaScriptedControlClass_Get(Is_Checked, ScriptedCheckBoxControlClass, boolean)
	LuaScriptedControlClass_Set(Set_Checked, ScriptedCheckBoxControlClass, boolean)

	/* ScriptedComboBoxControlClass */
	LuaScriptedControlClass_Get(Get_Selected_Index, ScriptedComboBoxControlClass, number)
	LuaScriptedControlClass_Get(Get_Item_Count, ScriptedComboBoxControlClass, number)
	static int Get_Item(lua_State *L);
	LuaScriptedControlClass_GetText(Get_Selected_Item_Text, ScriptedComboBoxControlClass)

	LuaScriptedControlClass_Set(Set_Selected_Index, ScriptedComboBoxControlClass, integer)
	LuaScriptedControlClass_Set(Remove_Item, ScriptedComboBoxControlClass, integer)
	LuaScriptedControlClass_SetText(Add_Item, ScriptedComboBoxControlClass)
	static int Insert_Item(lua_State *L);
	LuaScriptedControlClass_Call(Clear_Items, ScriptedComboBoxControlClass)

	/* ScriptedSliderControlClass */
	LuaScriptedControlClass_Get2(Get_Minimum, ScriptedSliderControlClass, ScriptedProgressBarControlClass, number)
	LuaScriptedControlClass_Get2(Get_Maximum, ScriptedSliderControlClass, ScriptedProgressBarControlClass, number)
	LuaScriptedControlClass_Get(Get_Value, ScriptedSliderControlClass, number)

	LuaScriptedControlClass_Set(Set_Value, ScriptedSliderControlClass, integer)
	LuaScriptedControlClass_Set2(Set_Maximum, ScriptedSliderControlClass, ScriptedProgressBarControlClass, integer)
	LuaScriptedControlClass_Set2(Set_Minimum, ScriptedSliderControlClass, ScriptedProgressBarControlClass, integer)

	/* ScriptedProgressBarControlClass */
	LuaScriptedControlClass_Get(Get_Progress, ScriptedProgressBarControlClass, number)
	LuaScriptedControlClass_Get(Get_Step_Count, ScriptedProgressBarControlClass, number)

	LuaScriptedControlClass_Call(Increment_Value, ScriptedProgressBarControlClass)
	LuaScriptedControlClass_Call(Decrement_Value, ScriptedProgressBarControlClass)
	LuaScriptedControlClass_Set(Set_Progress, ScriptedProgressBarControlClass, integer)
	LuaScriptedControlClass_Set(Set_Step_Count, ScriptedProgressBarControlClass, integer)

	
	/*static int DeleteInstance(lua_State *L)
	{
		ScriptedControlClass  *a = (ScriptedControlClass *)lua_unboxpointer(L, 1);
		delete a;
		return 0;
	}*/
public:
	static void Register(lua_State *L);

	static void Wrap(lua_State *L, ScriptedControlClass *ptr)
	{
		lua_boxpointer(L, ptr);
		luaL_getmetatable(L, "ScriptedControlClass");
		lua_setmetatable(L, -2);
	}

	static ScriptedControlClass *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		void *ud = luaL_checkudata(L, n, "ScriptedControlClass");
		if (!ud)
		{
			luaL_typerror(L, n, "ScriptedControlClass");
			return nullptr;
		}
		return *(ScriptedControlClass**)ud;
	}
};


class LuaScriptedDialogClass
{
	static const luaL_Reg methods[];

	static int Get_Dialog_Type(lua_State *L);
	static int Get_Dialog_ID(lua_State *L);
	static int Get_Client_ID(lua_State *L);
	static int Is_Dirty(lua_State *L);
	static int Set_Dirty(lua_State *L);

	static int Add_Control(lua_State *L);
	static int Create_Control(lua_State *L);
	static int Find_Control(lua_State *L);
	static int Get_Control_Count(lua_State *L);
	static int Get_Control_Index(lua_State *L);
	static int Remove_Control(lua_State *L);
	static int Clear_Controls(lua_State *L);

	/* ScriptedPopupDialogClass */
	static int Get_Orientation(lua_State *L);
	static int Get_Dialog_Size(lua_State *L);
	static int Get_Dialog_Location(lua_State *L);
	static int Get_Dialog_Title(lua_State *L);

	static int Set_Dialog_Size(lua_State *L);
	static int Set_Dialog_Location(lua_State *L);
	static int Set_Dialog_Title(lua_State *L);
	static int Set_Orientation(lua_State *L);

public:
	static void Register(lua_State *L);

	static void Wrap(lua_State *L, ScriptedDialogClass *ptr)
	{
		lua_boxpointer(L, ptr);
		luaL_getmetatable(L, "ScriptedDialogClass");
		lua_setmetatable(L, -2);
	}

	static ScriptedDialogClass *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		void *ud = luaL_checkudata(L, n, "ScriptedDialogClass");
		if (!ud)
		{
			luaL_typerror(L, n, "ScriptedDialogClass");
			return nullptr;
		}
		return *(ScriptedDialogClass**)ud;
	}
};



class LuaHUDSurfaceClass
{
	static const luaL_Reg methods[];

	static int Get_Surface_ID(lua_State *L);
	static int Get_Client_ID(lua_State *L);
	static int Get_Boundary_Area(lua_State *L);
	static int Get_Aspect_Ratio(lua_State *L);
	static int Get_Surface_Texture(lua_State *L);
	static int Set_Surface_Texture(lua_State *L);
	static int Is_Dirty(lua_State *L);
	static int Set_Dirty(lua_State *L);
	static int Create_Element(lua_State *L);
	static int Find_Element(lua_State *L);
	static int Get_Element_Count(lua_State *L);
	static int Get_Element_Index(lua_State *L);
	static int Remove_Element(lua_State *L);
	static int Clear_Elements(lua_State *L);

public:
	static void Register(lua_State *L);

	static void Wrap(lua_State *L, HUDSurfaceClass *ptr)
	{
		lua_boxpointer(L, ptr);
		luaL_getmetatable(L, "HUDSurfaceClass");
		lua_setmetatable(L, -2);
	}

	static HUDSurfaceClass *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		void *ud = luaL_checkudata(L, n, "HUDSurfaceClass");
		if (!ud)
		{
			luaL_typerror(L, n, "HUDSurfaceClass");
			return nullptr;
		}
		return *(HUDSurfaceClass**)ud;
	}
};

#define LuaHUDElementClass_GetVector2(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	auto num = sub->name(); \
	lua_pushnumber(L, num.X); \
	lua_pushnumber(L, num.Y); \
	return 2; \
} \
} \
	return 0; \
}

#define LuaHUDElementClass_SetVector2(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 3) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	Vector2 vec(lua_tonumber(L, 2), lua_tonumber(L, 3));\
	sub->name(vec); \
} \
} \
	return 0; \
}

#define LuaHUDElementClass_GetColor(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	auto num = sub->name(); \
	lua_pushnumber(L, num.X); \
	lua_pushnumber(L, num.Y); \
	lua_pushnumber(L, num.Z); \
	lua_pushnumber(L, num.W); \
	return 4; \
} \
} \
	return 0; \
}

#define LuaHUDElementClass_SetColor(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 5) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	sub->name(lua_tointeger(L, 2), lua_tointeger(L, 3), lua_tointeger(L, 4), lua_tointeger(L, 5)); \
} \
} \
	return 0; \
}

#define LuaHUDElementClass_SetNumber(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 2) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	sub->name(lua_tonumber(L, 2)); \
} \
} \
	return 0; \
}

#define LuaHUDElementClass_SetBool(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 2) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	sub->name(lua_toboolean(L, 2)); \
} \
} \
	return 0; \
}


#define LuaHUDElementClass_SetFont(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 2) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	sub->name(static_cast<HUDFontType>(lua_tointeger(L, 2))); \
} \
} \
	return 0; \
}

#define LuaHUDElementClass_Get(name, subclass, type) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	lua_push##type(L, sub->name()); \
	return 1; \
} \
} \
	return 0; \
}


#define LuaHUDElementClass_GetRect(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	auto rect = sub->name();\
	lua_newtable(L);\
	int tbl = lua_gettop(L);\
	int index = 1;\
	lua_pushnumber(L, rect.Left);\
	lua_rawseti(L, tbl, 1); \
	lua_pushnumber(L, rect.Top);\
	lua_rawseti(L, tbl, 2); \
	lua_pushnumber(L, rect.Right);\
	lua_rawseti(L, tbl, 3); \
	lua_pushnumber(L, rect.Bottom);\
	lua_rawseti(L, tbl, 4); \
	return 1; \
} \
} \
	return 0; \
}

#define LuaHUDElementClass_SetRect(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 2) return 0; \
	HUDElementClass *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
{ \
	if (!lua_istable(L, 2))\
    {\
		luaL_error(L, "Invalid argument #2. Expected a table.");\
		return 0;\
	}\
	float point[4];\
	for (int a = 1; a <= 4; a++)\
	{\
		lua_rawgeti(L, 2, a);\
		if (lua_type(L, -1) == LUA_TNUMBER)\
			point[a - 1] = lua_tonumber(L, -1);\
		lua_pop(L, 1);\
	}\
	RectClass rect(point[0], point[1], point[2], point[3]);\
	sub->name(rect);\
} \
} \
	return 0; \
}

#define LuaHUDElementClas_SetText(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 2) return 0; \
	auto *a = GetInstance(L, 1); \
if (a != nullptr) \
{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
	{ \
	auto str = lua_tostring(L, 2); \
	WideStringClass wch(str); \
	sub->name(wch); \
	} \
} \
	return 0; \
}

#define LuaHUDElementClas_GetText(name, subclass) static int name(lua_State *L) \
{ \
if (lua_gettop(L) < 1) return 0; \
	auto *a = GetInstance(L, 1); \
if (a != nullptr) \
	{ \
	auto sub = a->As_##subclass(); \
if (sub != nullptr) \
		{ \
		auto wch = sub->name(); \
		StringClass str; \
		wch.Convert_To(str); \
		lua_pushstring(L, str.Peek_Buffer()); \
		return 1; \
		} \
	} \
	return 0; \
}

#define LuaHUD_GetInstance(ClassName) static ClassName *GetInstance(lua_State *L, int n)\
{\
luaL_checktype(L, n, LUA_TUSERDATA);\
void *ud = luaL_checkudata(L, n, #ClassName);\
if (!ud)\
{\
	luaL_typerror(L, n, #ClassName);\
	return nullptr;\
}\
return *(ClassName**)ud;\
}

#define LuaHUD_Wrap(ClassName) static void Wrap(lua_State *L, ClassName *ptr)\
{\
	lua_boxpointer(L, ptr);\
	luaL_getmetatable(L, #ClassName); \
	lua_setmetatable(L, -2);\
}

#define LuaHUD_Register(ClassName) static void Register(lua_State *L)\
{\
	lua_newtable(L);\
	int methodtable = lua_gettop(L);\
	luaL_newmetatable(L, #ClassName); \
	int metatable = lua_gettop(L);\
	lua_pushliteral(L, "__metatable");\
	lua_pushvalue(L, methodtable);\
	lua_settable(L, metatable);\
	lua_pushliteral(L, "__index");\
	lua_pushvalue(L, methodtable);\
	lua_settable(L, metatable);\
	lua_pop(L, 1);\
	luaL_openlib(L, 0, methods, 0);\
	lua_pop(L, 1);\
}

class LuaHUDLineElementClass
{
	static const luaL_Reg methods[];
	LuaHUDElementClass_GetVector2(Get_P0, HUDLineElementClass)
	LuaHUDElementClass_GetVector2(Get_P1, HUDLineElementClass)
	LuaHUDElementClass_SetVector2(Set_P0, HUDLineElementClass)
	LuaHUDElementClass_SetVector2(Set_P1, HUDLineElementClass)
	LuaHUDElementClass_SetColor(Set_Color, HUDLineElementClass)
	LuaHUDElementClass_GetColor(Get_Color, HUDLineElementClass)
	LuaHUDElementClass_SetNumber(Set_Thickness, HUDLineElementClass)
	LuaHUDElementClass_Get(Get_Thickness, HUDLineElementClass, number)
	LuaHUDElementClass_Get(Get_Length, HUDLineElementClass, number)

public:
	LuaHUD_Register(HUDLineElementClass)
	LuaHUD_Wrap(HUDLineElementClass)
	LuaHUD_GetInstance(HUDLineElementClass)
};

class LuaHUDRectangleElementClass
{
	static const luaL_Reg methods[];
	LuaHUDElementClass_SetColor(Set_Color, HUDRectangleElementClass)
	LuaHUDElementClass_GetColor(Get_Color, HUDRectangleElementClass)
	LuaHUDElementClass_SetRect(Set_Rect, HUDRectangleElementClass)
	LuaHUDElementClass_GetRect(Get_Rect, HUDRectangleElementClass)
	LuaHUDElementClass_Get(Get_Area, HUDRectangleElementClass, number)
	LuaHUDElementClass_Get(Get_Diagonal_Length, HUDRectangleElementClass, number)

public:
	LuaHUD_Register(HUDRectangleElementClass)
	LuaHUD_Wrap(HUDRectangleElementClass)
	LuaHUD_GetInstance(HUDRectangleElementClass)
};

class LuaHUDOutlineElementClass
{
	static const luaL_Reg methods[];
	LuaHUDElementClass_SetColor(Set_Color, HUDOutlineElementClass)
	LuaHUDElementClass_GetColor(Get_Color, HUDOutlineElementClass)
	LuaHUDElementClass_SetRect(Set_Rect, HUDOutlineElementClass)
	LuaHUDElementClass_GetRect(Get_Rect, HUDOutlineElementClass)
	LuaHUDElementClass_Get(Get_Area, HUDOutlineElementClass, number)
	LuaHUDElementClass_Get(Get_Diagonal_Length, HUDOutlineElementClass, number)
	LuaHUDElementClass_SetNumber(Set_Thickness, HUDOutlineElementClass)
	LuaHUDElementClass_Get(Get_Thickness, HUDOutlineElementClass, number)

public:
	LuaHUD_Register(HUDOutlineElementClass)
	LuaHUD_Wrap(HUDOutlineElementClass)
	LuaHUD_GetInstance(HUDOutlineElementClass)
};


class LuaHUDTextElementClass
{
	static const luaL_Reg methods[];
	LuaHUDElementClass_SetColor(Set_Color, HUDTextElementClass)
	LuaHUDElementClass_GetColor(Get_Color, HUDTextElementClass)
	LuaHUDElementClass_SetRect(Set_Clipping_Area, HUDTextElementClass)
	LuaHUDElementClass_GetRect(Get_Clipping_Area, HUDTextElementClass)
	LuaHUDElementClass_Get(Get_Clipping_Area_Size, HUDTextElementClass, number)
	LuaHUDElementClass_Get(Get_Clipping_Area_Diagonal_Length, HUDTextElementClass, number)
	LuaHUDElementClas_SetText(Set_Text, HUDTextElementClass)
	LuaHUDElementClas_GetText(Get_Text, HUDTextElementClass)
	LuaHUDElementClass_Get(Get_Font, HUDTextElementClass, number)
	LuaHUDElementClass_SetFont(Set_Font, HUDTextElementClass)

public:
	LuaHUD_Register(HUDTextElementClass)
	LuaHUD_Wrap(HUDTextElementClass)
	LuaHUD_GetInstance(HUDTextElementClass)
};

class LuaHUDElementClass
{
	friend class LuaHUDLineElementClass;
	friend class LuaHUDRectangleElementClass;
	friend class LuaHUDOutlineElementClass;
	friend class LuaHUDTextElementClass;

	/* HUDElementClass */
	static int Get_Element_Type(lua_State *L);
	static int Get_Element_ID(lua_State *L);
	static int Is_Dirty(lua_State *L);
	static int Set_Dirty(lua_State *L);
	static int Is_Rendered(lua_State *L);
	static int Set_Rendered(lua_State *L);
	static int Get_UV_Range(lua_State *L);
	static int Set_UV_Range(lua_State *L);

	static HUDElementClass *GetInstance(lua_State *L, int n)
	{
		luaL_checktype(L, n, LUA_TUSERDATA);
		if (luaL_checkudata(L, n, "HUDLineElementClass") != nullptr)
			return LuaHUDLineElementClass::GetInstance(L, n);
		if (luaL_checkudata(L, n, "HUDRectangleElementClass") != nullptr)
			return LuaHUDRectangleElementClass::GetInstance(L, n);
		if (luaL_checkudata(L, n, "HUDOutlineElementClass") != nullptr)
			return LuaHUDOutlineElementClass::GetInstance(L, n);
		if (luaL_checkudata(L, n, "HUDTextElementClass") != nullptr)
			return LuaHUDTextElementClass::GetInstance(L, n);

		return nullptr;
	}

public:
	static void Register(lua_State *L);
	
};

#endif