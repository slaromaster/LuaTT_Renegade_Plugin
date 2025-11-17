if #arg < 2 then print("Format is <input> <output>"); return end


local i = assert(io.open(arg[1], "r"))
print("Reading..")
local foundLuaFunction = false;
local brackets = 0;
local LineNumber= 0
local start = os.clock();

local functions = {}

-- Reading Part

function populateWithUnknownParams(m_table, m_total)
	for a = 1, m_total do
		m_table[a]="unknown_"..a;
	end
end

local string_capture = {"luaL_checkstring%s*%(%s*L%s*,%s*(%d)%s*%)", "lspop%s*%(%s*L%s*,%s*(%d)%s*%)", "lua_tostring%s*%(%s*L%s*,%s*(%d)%s*%)", "luaL_checklstring%s*%(%s*L%s*,%s*(%d)%s*%)", "luaL_optlstring%s*%(%s*L%s*,%s*(%d)%s*%)", "lua_tolstring%s*%(%s*L,%s*(%d)%s*%)" }
local number_capture = {"lnpop%s*%(%s*L%s*,%s*(%d)%s*%)", "lua_tonumber%s*%(%s*L%s*,%s*(%d)%s*%)", "luaL_checknumber%s*%(%s*L%s*,%s*(%d)%s*%)", "luaL_checkinteger%s*%(%s*L%s*,%s*(%d)%s*%)", "lua_tointeger%s*%(%s*L%s*,%s*(%d)%s*%)", "luaL_checkint%s*%(%s*L%s*,%s*(%d)%s*%)", "luaL_checklong%s*%(%s*L%s*,%s*(%d)%s*%)"}
local bool_capture = {"lua_toboolean%s*%(%s*L%s*,%s*(%d)%s*%)", "lua_checkboolean%s*%(%s*L%s*,%s*(%d)%s*%)", "lua_tobooleanCPP%s*%(%s*L%s*,%s*(%d)%s*%)"}
local vector3_capture = {"LuaVector3::GetInstance%s*%(%s*L%s*,%s*(%d)%s*%)",}

local Line
local func_name



function doCaptureCheck(capture, Line2, functions, func_name, type, unknown)
	local cap = nil;
	local Line = nil
	for i,v in ipairs(capture) do
		Line = Line2;
		while true do
			cap = Line:match(v)
			if not cap then break end
			if tonumber(cap) then
				functions[func_name].params[tonumber(cap)] = type;
			end
			local s,e = Line:find(v);
			if e then
				Line = Line:sub(e+1, -1)
			else
				Line = ""
			end
		end
	end
end


	

while true do
	Line = i:read("*line")
	if not Line then break end
	LineNumber = LineNumber + 1;
	if foundLuaFunction == false then
		if (Line:match("Lua_") or Line:match("LuaDA_")) and (Line:match("%(%s*lua_State%s*%*%s*L%s*%)")) then
			foundLuaFunction = true;
			local str = "Lua_"
			local n = 4;
			if Line:match("LuaDA_") then str = "LuaDA_"; n = 3; end
			func_name = Line:sub(Line:find(str)+n, Line:find("%(lua_")-1)
			--print("Found Function",func_name)
			functions[func_name] = {param_num=0, params={}}
		elseif (Line:match("LUA_IS_OBJ") or Line:match("LUA_IS_PHYS")) then
			local str = "LUA_IS_OBJ"
			local n = 11;
			if Line:match("LUA_IS_PHYS") then str = "LUA_IS_PHYS"; n = 12; end
			func_name = Line:sub(Line:find(str)+n, Line:find(",")-1)
			--print("Found Function",func_name)
			functions[func_name] = {param_num=1, params={}}
			functions[func_name].params[1] = "number";
		end
	else
		if (Line:match("Lua_") or Line:match("LuaDA_")) and (Line:match("%(%s*lua_State%s*%*%s*L%s*%)")) then
			error(Line.." #"..LineNumber)
		elseif (Line:match("LUA_IS_OBJ") or Line:match("LUA_IS_PHYS")) then
			error(Line.." #"..LineNumber)
		end
	
		if Line:match("{") then
			brackets = brackets + 1;
		end
		if Line:match("}") then
			brackets = brackets - 1;
			if brackets < 0 then
				error("Too many } found at: #"..LineNumber);
			elseif brackets == 0 then
				foundLuaFunction = false;
			end
		end
		
		if brackets > 0 then
			if Line:match("%(%s*lua_gettop%s*%(%s*L%s*%)%s*<%s*%d%s*%)") then
				local num = Line:match("%(%s*lua_gettop%s*%(%s*L%s*%)%s*<%s*(%d)%s*%)")
				if tonumber(num) then
					functions[func_name].param_num = tonumber(num)
					populateWithUnknownParams(functions[func_name].params, num);
				else
					print("Error?", num, LineNumber, Line);
				end
			else
				doCaptureCheck(string_capture, Line, functions, func_name, "string")
				doCaptureCheck(number_capture, Line, functions, func_name, "number")
				doCaptureCheck(bool_capture, Line, functions, func_name, "bool")
				doCaptureCheck(vector3_capture, Line, functions, func_name, "userdata")
			end
		end
	end
end

i:close();

local n = 0
for i,v in pairs(functions) do n = n + 1; end
print("Got #"..n)

-- Writing Part
local wr  = assert(io.open(arg[2], "w"))
local format = [["FUNC_NAME": {
  "prefix": "FUNC_NAME",
  "body": [
    "FUNC_NAME(PARAMS)"
  ],
  "description": "FUNC_NAME"
},
]]

local format_param = "${ID:NAME}"
local separator_param = ", "

local towrite = "{"
local temp = ""
local param_temp = ""
local c = 0
for name,data in pairs(functions) do
	c = c + 1
	param_temp = ""
	temp = ""

	temp = format:gsub("FUNC_NAME", name)
	for i,v in pairs(data.params) do
		local str = format_param:gsub("ID", i):gsub("NAME", v)
		if param_temp == "" then
			param_temp = str
		else
			param_temp = param_temp .. separator_param .. str
		end
	end
	temp = temp:gsub("PARAMS", param_temp)
	if c == n then
		temp = temp:gsub("},", "}")
	end
	towrite = towrite .."\n"..temp
end
towrite = towrite .. "}"

wr:write(towrite);
wr:close()

print("Done! "..os.clock()-start)

print(unpack(arg))