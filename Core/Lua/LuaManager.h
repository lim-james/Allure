#pragma once

#include "SingletonTemplate.h"
// Lua's headers
#include "../Lua/lua.hpp"
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class CLuaManager : public Singleton<CLuaManager>
{
public:
	// Constructor
	CLuaManager(void);

	// Destructor
	virtual ~CLuaManager(void);

	// Initialise this class instance
	bool Init(void);
	// Initialise this class instance - overloaded
	bool Init(const string& filename, const std::string& Writefilename, const bool bDisplayFileContent = false);
	// Destroy this class instance
	void Destroy(void);
	// PrintSelf
	void PrintSelf(void);
	// Print error while using Lua functions
	void printError(const string& variableName, const string& reason);

	// Get an Vector of Integers
	vector<int> getIntVector(const string& name);
	// Get an Vector of String
	void getStringVector(const std::string& name, std::vector<string>& keys, std::vector<int>& values);
	// Get a table of keys
	vector<string> getTableKeys(const string& name);

	// Clean the Lua State
	inline void clean(void);

	// Generic get method
	template<typename T>
	T get(const std::string& variableName)
	{
		if (!pLuaState) {
			printError(variableName, "Script is not loaded");
			return lua_getdefault<T>();
		}

		T result;
		// Check if the variable exists in the Lua file
		if (lua_gettostack(variableName))
		{
			// variable succesfully on top of stack
			/* ADD CODES HERE */
		}
		else 
		{
			/* ADD CODES HERE */
		}

		clean();
		return result;
	}

	// Get to Stack
	bool lua_gettostack(const std::string& variableName);

	// Generic get which return 0. This is a Template
	template<typename T>
	T lua_get(const std::string& variableName) {
		return 0;
	}

	// Generic default get which return 0. This is a Template
	template<typename T>
	T lua_getdefault() {
		return 0;
	}

	// Generic set method
	template<typename T>
	bool set(const std::string& variableName, const T value, const bool bOverwrite = true)
	{
		if (!pLuaState) {
			printError(variableName, "Script is not loaded");
			return false;
		}

		T result = false;
		// If overwriting existing variable, then don't check if it exists in the Lua file
		if (bOverwrite)
		{
			// variable succesfully on top of stack
			/* ADD CODES HERE */
		}
		else
		{
			// Check if the variable exists in the Lua file
			if (lua_gettostack(variableName))
			{
				// variable succesfully on top of stack
				/* ADD CODES HERE */
			}
		}

		clean();
		return result;
	}

	// Generic set. This is a Template
	template<typename T>
	bool lua_set(const std::string& variableName, const T value, const bool bOverwrite = true) {
		return false;
	}

	// Generic default set. This is a Template
	template<typename T>
	T lua_setdefault() {
		return false;
	}

protected:
	// Check if the Lua State is valid
	bool isLuaStateValid(void);
	// Stack dump
	void stackDump(void);

	// Lua State for Reading In
	lua_State* pLuaState;
	// Lua State for Writing Out
	lua_State* pWriteLuaState;
	std::string filename;
	int level;
};

/********************************************************************************/
// Helper template methods for GET
/********************************************************************************/
template <>
inline bool CLuaManager::lua_get<bool>(const std::string& variableName)
{
	return (bool)lua_toboolean(pLuaState, -1);
}

template <>
inline float CLuaManager::lua_get<float>(const std::string& variableName)
{
	if (!lua_isnumber(pLuaState, -1)) {
		printError(variableName, "Not a number");
	}
	return (float)lua_tonumber(pLuaState, -1);
}

template <>
inline int CLuaManager::lua_get<int>(const std::string& variableName)
{
	if (!lua_isnumber(pLuaState, -1)) {
		printError(variableName, "Not a number");
	}
	return (int)lua_tonumber(pLuaState, -1);
}

template <>
inline std::string CLuaManager::lua_get<std::string>(const std::string& variableName)
{
	std::string s = "null";
	if (lua_isstring(pLuaState, -1)) {
		s = std::string(lua_tostring(pLuaState, -1));
	}
	else {
		printError(variableName, "Not a string");
	}
	return s;
}

template<>
inline std::string CLuaManager::lua_getdefault<std::string>()
{
	return "null";
}

/********************************************************************************/
// Helper template methods for SET
/********************************************************************************/
template <>
inline bool CLuaManager::lua_set<bool>(const std::string& variableName, const bool bValue, const bool bOverwrite)
{
	/* ADD CODES HERE */

	return true;
}

template <>
inline bool CLuaManager::lua_set<float>(const std::string& variableName, const float fValue, const bool bOverwrite)
{
	/* ADD CODES HERE */

	return true;
}

template <>
inline bool CLuaManager::lua_set<int>(const std::string& variableName, const int iValue, const bool bOverwrite)
{
	/* ADD CODES HERE */

	return true;
}

template <>
inline bool CLuaManager::lua_set<std::string>(const std::string& variableName, const std::string sValue, const bool bOverwrite)
{
	/* ADD CODES HERE */

	return true;
}

template<>
inline std::string CLuaManager::lua_setdefault<std::string>() {
	return "null";
}
