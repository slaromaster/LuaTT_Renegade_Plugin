/*	Lua Thread
Adds multithreading to LuaTT
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2023 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/
#include "General.h"
#include "luatt.h"
#include "LuaThread.h"

#include "LuaFunctions.h"

#ifdef DAPLUGIN
#include "LuaFunctionsDA.h"
#endif


#define THREAD_ID_LUA "THREAD_ID"
std::mutex LuaThread::m_locker;
std::mutex LuaThread::m_lockerFDS;
std::condition_variable_any LuaThread::m_newMessage;
std::condition_variable_any LuaThread::m_lockCheck;
bool LuaTT_threadEnabled = false;

/* LuaThreadInternal */
LuaDataMessage::LuaDataMessage(const char *string)
{
	type = LUA_TYPE_STRING;
	data = reinterpret_cast<void*>(_strdup(string));
}

LuaDataMessage::LuaDataMessage(double number)
{
	type = LUA_TYPE_NUMBER;
	data = reinterpret_cast<void*>(new double(number));
}

LuaDataMessage::LuaDataMessage(bool boolValue)
{
	type = LUA_TYPE_BOOLEAN;
	data = reinterpret_cast<void*>(new bool(boolValue));
}

LuaDataMessage::LuaDataMessage(TableType &list)
{
	type = LUA_TYPE_TABLE;
	data = reinterpret_cast<void*>(new TableType(list));
}

LuaDataMessage::~LuaDataMessage()
{
	delete data;
	data = nullptr;
}

const char *LuaDataMessage::toString()
{
	if (type == LUA_TYPE_STRING)
	{
		return reinterpret_cast<const char *>(data);
	}

	throw std::runtime_error("Not a string");
}

double LuaDataMessage::toNumber()
{
	if (type == LUA_TYPE_NUMBER)
	{
		return *reinterpret_cast<double *>(data);
	}

	throw std::runtime_error("Not a number");
}

bool LuaDataMessage::toBool()
{
	if (type == LUA_TYPE_BOOLEAN)
	{
		return *reinterpret_cast<bool *>(data);
	}

	throw std::runtime_error("Not a boolean");
}

LuaDataMessage::TableType *LuaDataMessage::toTable()
{
	if (type == LUA_TYPE_TABLE)
	{
		return reinterpret_cast<TableType *>(data);
	}

	throw std::runtime_error("Not a table");
}


/* LuaThreadInternal */

LuaThreadInternal::LuaThreadInternal(int id, const char *code)
: m_id(id)
{
	if (LuaTT_threadEnabled)
	{
		m_thread = new std::thread(executeThread, std::string(code), id);
		m_error = NULL;
		m_running = true;
	}
	else
	{
		m_thread = nullptr;
		m_error = "Cannot create thread while initialization is not done";
		m_running = false;
	}
}

LuaThreadInternal::~LuaThreadInternal()
{
	delete m_error.exchange(nullptr);
	if (m_thread != nullptr)
	{
		free(m_thread); /* we don't call delete because the destructor causes crash */
	}
}

void LuaThreadInternal::waitThread()
{
	if (m_thread->joinable())
	{
		m_thread->join();
	}
}

void LuaThreadInternal::setRunning(bool running)
{
	m_running = running;
}

void LuaThreadInternal::setError(const char *string)
{
	m_error = _strdup(string);
}

void LuaThreadInternal::executeThread(std::string luaCode, int id)
{
	const char *errorResult = NULL;
	lua_State *L = lua_open();
	luaL_openlibs(L);
	LuaThread::Register_Lua_Thread(L, true);
	lua_pushnumber(L, id);
	lua_setglobal(L, THREAD_ID_LUA);

	if (luaL_loadbuffer(L, luaCode.c_str(), luaCode.size(), "thread") == 0) /* Load Lua Code */
	{
		int status = lua_pcall(L, 0, 0, 0); /* Run Lua Code */
		if (status != 0)
		{
			const char *err = lua_tostring(L, -1);
			lua_pop(L, 1);
			errorResult = err;
		}
	}
	else
	{
		const char *err = lua_tostring(L, -1);
		lua_pop(L, 1);
		errorResult = err;
	}


	
	if (errorResult != NULL)
	{
		LuaThread::setThreadExit(id, THREAD_EXIT_ERROR, errorResult);
	}
	else
	{
		LuaThread::setThreadExit(id, THREAD_EXIT_OK, "");
	}

	lua_close(L);
}

/* LuaThread */

LuaThread::LuaThread()
{
}

LuaThread *LuaThread::getInstance()
{
	static LuaThread s_instance;
	return &s_instance;
}

int LuaThread::getNewID()
{
	m_id++;
	return m_id;
}

int LuaThread::Lua_New_Thread(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	const char * str = luaL_checkstring(L, 1);
	if (str == NULL)
	{
		return 0;
	}
	auto inst = getInstance();
	std::lock_guard<std::mutex> guard(m_locker);
	inst->CheckDeleteLater();

	int id = inst->getNewID();
	inst->m_threads.push_back(std::make_shared<LuaThreadInternal>(id, str));
	lua_pushnumber(L, id);

	return 1;
}

int LuaThread::Lua_Wait_Thread(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	int threadId = luaL_checkint(L, 1);
	LuaThread *inst = getInstance();
	m_locker.lock();
	auto it = std::find_if(inst->m_threads.begin(), inst->m_threads.end(), [&threadId](std::shared_ptr<LuaThreadInternal> &obj){ return obj->getId() == threadId; });
	
	if (it != inst->m_threads.end())
	{
		m_locker.unlock();
		(*it)->waitThread();
	}
	else
	{
		m_locker.unlock();
	}

	return 0;
}

int LuaThread::Lua_Is_Thread_Running(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	int threadId = luaL_checkint(L, 1);

	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);
	inst->CheckDeleteLater();

	auto it = std::find_if(inst->m_threads.begin(), inst->m_threads.end(), [&threadId](std::shared_ptr<LuaThreadInternal> &obj){ return obj->getId() == threadId; });
	if (it != inst->m_threads.end())
	{
		lua_pushboolean(L, (*it)->isRunning());
		return 1;
	}

	
	return 0;
}

int LuaThread::Lua_Get_Thread_Error(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	int threadId = luaL_checkint(L, 1);
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);

	auto it = std::find_if(inst->m_threads.begin(), inst->m_threads.end(), [&threadId](std::shared_ptr<LuaThreadInternal> &obj){ return obj->getId() == threadId; });
	if (it != inst->m_threads.end())
	{
		const char *err = (*it)->getError();
		if (err != NULL)
		{
			
			lua_pushstring(L, err);
			return 1;
		}
	}

	

	return 0;
}

int LuaThread::Lua_Remove_Thread(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	int threadId = luaL_checkint(L, 1);
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);

	auto it = std::find_if(inst->m_threads.begin(), inst->m_threads.end(), [&threadId](std::shared_ptr<LuaThreadInternal> &obj){ return obj->getId() == threadId; });
	if (it != inst->m_threads.end())
	{
		(*it)->shouldDelete();
	}

	inst->CheckDeleteLater();
	
	return 0;
}

int LuaThread::Lua_Get_Current_Thread(lua_State* L)
{
	lua_getglobal(L, THREAD_ID_LUA);
	if (lua_isnumber(L, -1))
	{
		lua_pushnumber(L, lua_tonumber(L, -1));
		return 1;
	}
	
	return 0;
}

LuaDataMessage *LuaThread::convertData(lua_State* L, int n, bool insideTable)
{
	LuaDataMessage *msg = nullptr;
	if (lua_isnumber(L, n) == TRUE)
	{
		msg = new LuaDataMessage(lua_tonumber(L, n));
	}
	else if (lua_isstring(L, n) == TRUE)
	{
		msg = new LuaDataMessage(lua_tostring(L, n));
	}
	else if (lua_isboolean(L, n) == TRUE)
	{
		msg = new LuaDataMessage(static_cast<bool>(lua_toboolean(L, n)));
	}
	else if (lua_istable(L, n) == TRUE)
	{
		std::map<LuaDataMessage*, LuaDataMessage*> map; /* have map prepared */
		if (insideTable)
		{
			n = lua_gettop(L);
		}
		lua_pushnil(L);  /* first key */
		
		while (lua_next(L, n) != 0)
		{
			/* Convert both key and value */
			LuaDataMessage *key = convertData(L, -2, true); 
			LuaDataMessage *value = convertData(L, -1, true);
			if (key != nullptr && value != nullptr)
			{
				map[key] = value;
			}

			/* removes 'value' to keep 'key' for next iteration */
			lua_pop(L, 1);
		}
		msg = new LuaDataMessage(map);
	}

	return msg;
}

int LuaThread::Lua_Send_Data(lua_State* L)
{
	if (lua_gettop(L) < 2) return 0;
	std::string channel = std::string(luaL_checkstring(L, 1));
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);

	LuaDataMessage *data = convertData(L, 2);
	if (data != nullptr)
	{
		inst->getChannel(channel)->push(data);
		lua_pushboolean(L, true);
		m_newMessage.notify_all();
	}
	else
	{
		lua_pushboolean(L, false);
	}

	return 1;
}

int LuaThread::Lua_Get_Data(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	std::string channelText = std::string(luaL_checkstring(L, 1));
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);
	auto channel = inst->getChannel(channelText);

	if (channel->size() <= 0)
	{
		return 0;
	}

	auto data = channel->front();
	int i = ReturnDataLua(L, data);
	channel->pop();
	delete data;
	
	return i;
}

int LuaThread::Lua_Get_All_Data(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	std::string channelText = std::string(luaL_checkstring(L, 1));
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);
	auto channel = inst->getChannel(channelText);

	lua_newtable(L);
	int k = 1;
	while (channel->size() > 0)
	{
		lua_pushnumber(L, k);
		auto data = channel->front();
		int i = ReturnDataLua(L, data);
		channel->pop();
		delete data;
		lua_settable(L, -3);
		k++;
	}
	
	return 1;
}

int LuaThread::Lua_Peek_Data(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	std::string channelText = std::string(luaL_checkstring(L, 1));
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);
	auto channel = inst->getChannel(channelText);

	if (channel->size() <= 0)
	{
		
		return 0;
	}

	auto data = channel->front();
	int i = ReturnDataLua(L, data);

	
	return i;
}


int LuaThread::Lua_Peek_All_Data(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	std::string channelText = std::string(luaL_checkstring(L, 1));
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);
	auto channel = inst->getChannel(channelText);

	lua_newtable(L);
	int k = 1;
	auto container = channel->_Get_container();
	for (auto data : container)
	{
		lua_pushnumber(L, k);
		int i = ReturnDataLua(L, data);
		lua_settable(L, -3);
		k++;
	}

	return 1;
}

int LuaThread::Lua_Demand_Data(lua_State* L)
{
	if (lua_gettop(L) < 1) return 0;
	std::string channelText = std::string(luaL_checkstring(L, 1));
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);
	auto channel = inst->getChannel(channelText);

	while (channel->size() <= 0)
	{
		m_newMessage.wait(m_locker);
	}

	auto data = channel->front();
	int i = ReturnDataLua(L, data);
	channel->pop();
	delete data;
	
	return i;
}

int LuaThread::Lua_Get_All_Channels(lua_State* L)
{
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);
	auto channels = inst->m_messages;

	lua_newtable(L);
	int k = 1;
	for (auto it = channels.begin(); it != channels.end(); ++it)
	{
		lua_pushnumber(L, k);
		lua_pushstring(L, it->first.c_str());
		lua_settable(L, -3);
		k++;
	}

	return 1;
}

int LuaThread::ReturnDataLua(lua_State* L, LuaDataMessage *data)
{
	int i = 0;
	auto type = data->getType();

	if (type == LUA_TYPE_NUMBER)
	{
		lua_pushnumber(L, data->toNumber());
		i++;
	}
	else if (type == LUA_TYPE_STRING)
	{
		lua_pushstring(L, data->toString());
		i++;
	}
	else if (type == LUA_TYPE_BOOLEAN)
	{
		lua_pushboolean(L, data->toBool());
		i++;
	}
	else if (type == LUA_TYPE_TABLE)
	{
		auto map = data->toTable();
		lua_createtable(L, 0, map->size());
		for (auto it = map->begin(); it != map->end(); it++)
		{
			ReturnDataLua(L, it->first); // key
			ReturnDataLua(L, it->second); // value
			lua_settable(L, -3);
		}
		i++;
	}

	return i;
}

void LuaThread::CheckDeleteLater()
{
	m_threads.erase(std::remove_if(m_threads.begin(), m_threads.end(), [](std::shared_ptr<LuaThreadInternal> &obj)
	{
		if (obj->shouldDelete())
		{
			return true;
		}
		else
		{
			return false;
		}
	}), m_threads.end());
}

LuaThread::QueueTable *LuaThread::getChannel(const std::string &msg)
{
	if (m_messages.find(msg) == m_messages.end())
	{
		m_messages.insert(std::pair<std::string, QueueTable>(msg, QueueTable()));
	}

	return (&(*m_messages.find(msg)).second);
}

void LuaThread::setThreadExit(int id, THREAD_EXIT_STATUS status, const char *error)
{
	auto inst = getInstance();
	std::unique_lock<std::mutex> guard(m_locker);
	auto it = std::find_if(inst->m_threads.begin(), inst->m_threads.end(), [&id](std::shared_ptr<LuaThreadInternal> &obj){ return obj->getId() == id; });
	if (it != inst->m_threads.end())
	{
		if (status == THREAD_EXIT_ERROR)
		{
			(*it)->m_error = _strdup(error);
			(*it)->setRunning(false);
		}
		else
		{
			(*it)->setRunning(false);
			(*it)->deleteLater();
		}
	}
}

int LuaThread::Lua_Lock(lua_State* L)
{
	/* FDS main thread will hang while our lock is in place
	  lock and wait for lock to happen successfully,
	  other threads attempting to lock will wait until we unlock
	*/
	m_lockerFDS.lock();
	m_lockCheck.wait(m_lockerFDS);

	return 0;
}

int LuaThread::Lua_Unlock(lua_State* L)
{
	/* FDS main thread can now continue */
	m_lockerFDS.unlock();
	return 0;
}

int LuaThread::Lua_Load_FDS_Functions(lua_State* L)
{
	AddFunctions(L);
#ifdef DAPLUGIN
	AddFunctionsDA(L);
#endif

	return 0;
}

void LuaThread::Lock_Check()
{
	m_lockCheck.notify_all();
	bool success = m_lockerFDS.try_lock();
	if (!success) /* we are locked, we must wait and notify thread to continue */
	{
		m_lockerFDS.lock();
	}
	m_lockerFDS.unlock();
}

void LuaThread::Register_Lua_Thread(lua_State* L, bool isThread)
{
	lua_register(L, "New_Thread", Lua_New_Thread);
	lua_register(L, "Wait_Thread", Lua_Wait_Thread);
	lua_register(L, "Is_Thread_Running", Lua_Is_Thread_Running);
	lua_register(L, "Get_Thread_Error", Lua_Get_Thread_Error);
	lua_register(L, "Remove_Thread", Lua_Remove_Thread);
	lua_register(L, "Get_Current_Thread", Lua_Get_Current_Thread);
	lua_register(L, "Send_Data", Lua_Send_Data);
	lua_register(L, "Get_Data", Lua_Get_Data);
	lua_register(L, "Get_All_Data", Lua_Get_All_Data);
	lua_register(L, "Peek_Data", Lua_Peek_Data);
	lua_register(L, "Peek_All_Data", Lua_Peek_All_Data);
	lua_register(L, "Demand_Data", Lua_Demand_Data);
	lua_register(L, "Get_All_Channels", Lua_Get_All_Channels);

	if (isThread)
	{
		lua_register(L, "Lock", Lua_Lock);
		lua_register(L, "Unlock", Lua_Unlock);
		lua_register(L, "Load_FDS_Functions", Lua_Load_FDS_Functions);
	}
}