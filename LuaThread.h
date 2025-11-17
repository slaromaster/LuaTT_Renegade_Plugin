/*	Lua Thread header
by Stan "sla.ro" Laurentiu Alexandru
Copyright 2010-2023 Sla Studios (http://slastudios.net)

This file is part of the LuaTT
*/

#include <list>
#include <map>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "LuaLib\lua.hpp"

enum LUA_DATA_TYPES
{
	LUA_TYPE_NUMBER, // double
	LUA_TYPE_BOOLEAN, // bool
	LUA_TYPE_STRING, // char*
    LUA_TYPE_TABLE // map
};

enum THREAD_EXIT_STATUS
{
	THREAD_EXIT_OK,
	THREAD_EXIT_ERROR
};

extern bool LuaTT_threadEnabled;

class LuaDataMessage
{
public:
	using TableType = std::map<LuaDataMessage*, LuaDataMessage*>;

	LuaDataMessage(const char *string);
	LuaDataMessage(double number);
	LuaDataMessage(TableType &list);
	LuaDataMessage(bool boolValue);
	~LuaDataMessage();

	LUA_DATA_TYPES getType() const { return type; }
	void *getRawData() const { return data; }

	const char *toString();
	double toNumber();
	TableType *toTable();
	bool toBool();

private:
	LUA_DATA_TYPES type = LUA_TYPE_NUMBER;
	void *data = NULL;
};

class LuaThreadInternal
{
public:
	friend class LuaThread;

	explicit LuaThreadInternal(int id, const char *code);
	~LuaThreadInternal();

	bool isRunning() const { return m_running; }
	const char *getError() const { return m_error; }
	int getId() const { return m_id; }
	void waitThread();
	void deleteLater() { m_delete = true; }
	bool shouldDelete() const { return m_delete;  }

private:
	static void executeThread(std::string luaCode, int id);

	/* Thread Safe */
	void setRunning(bool running);
	void setError(const char *error);

	std::thread *m_thread;
	std::atomic<int> m_id;
	std::atomic<bool> m_running;
	std::atomic<const char*> m_error;
	std::atomic<bool> m_delete;
};


class LuaThread
{
public:
	friend class LuaThreadInternal;
	using QueueTable = std::queue<LuaDataMessage*>;

	explicit LuaThread();
	static LuaThread *getInstance();


	static void Register_Lua_Thread(lua_State* L, bool isThread = false);
	int getNewID();
	void CheckDeleteLater();
	static void Lock_Check();
	QueueTable *getChannel(const std::string &msg);
	static LuaDataMessage *convertData(lua_State* L, int n, bool insideTable = false);

private:

	static int ReturnDataLua(lua_State* L, LuaDataMessage *data);
	static void setThreadExit(int id, THREAD_EXIT_STATUS status, const char *error);

	/* Lua Functions */
	static int Lua_New_Thread(lua_State* L);
	static int Lua_Wait_Thread(lua_State* L);
	static int Lua_Is_Thread_Running(lua_State* L);
	static int Lua_Get_Thread_Error(lua_State* L);
	static int Lua_Remove_Thread(lua_State* L);
	static int Lua_Get_Current_Thread(lua_State* L);
	static int Lua_Send_Data(lua_State* L);
	static int Lua_Get_Data(lua_State* L);
	static int Lua_Peek_Data(lua_State* L);
	static int Lua_Demand_Data(lua_State* L);
	static int Lua_Load_FDS_Functions(lua_State* L);
	static int Lua_Unlock(lua_State* L);
	static int Lua_Lock(lua_State* L);
	static int Lua_Get_All_Data(lua_State* L);
	static int Lua_Peek_All_Data(lua_State* L);
	static int Lua_Get_All_Channels(lua_State* L);

	/* Static variables */
	static std::mutex m_locker;
	static std::mutex m_lockerFDS;
	static std::condition_variable_any m_newMessage;
	static std::condition_variable_any m_lockCheck;

	/* Objects */
	std::list<std::shared_ptr<LuaThreadInternal>> m_threads;
	std::map<std::string, QueueTable> m_messages;
	
	unsigned int m_id = 0;

};