#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include "lua.hpp"

#include <Logger/Logger.h>

#include <string>
#include <vector>

class LuaScript {

	lua_State* L;
	int level;

public:

	LuaScript(const std::string& filepath);
	~LuaScript();

	void Clean();

	template<typename T>
	T Get(const std::string& identifier);

	template<typename T>
	bool Set(const std::string& identifier, const T& value, LuaScript * const writer);

	template<typename T>
	std::vector<T> GetVector(const std::string& identifer);

	std::vector<std::string> GetKeys(const std::string& identifier);

private:

	bool LuaGetToStack(const std::string& identifier);

	template<typename T>
	T LuaGetDefault() {
		return 0;
	};

	template<typename T>
	T LuaGet(const std::string& identifier) {
		return 0;
	}

	template<typename T>
	bool LuaSet(const std::string& identifier, const T& value) {
		return false;
	}

	void PrintError(const std::string& identifier, const std::string& reason) const;

};

template<typename T>
T LuaScript::Get(const std::string & identifier) {
	if (!L) {
		Console::Error << "Script not loaded\n";
		return LuaGetDefault<T>();
	}

	T result;

	if (LuaGetToStack(identifier)) {
		result = LuaGet<T>(identifier);
	} else {
		result = LuaGetDefault<T>();
	}

	Clean();
	return result;
}

template<typename T>
bool LuaScript::Set(const std::string & identifier, const T& value, LuaScript * const writer) {
	if (!L) {
		Console::Error << "Script not loaded\n";
		return false;
	}

	bool result = false;

	if (LuaGetToStack(identifier)) {
		result = LuaSet<T>(identifier, value);

		//lua_getglobal(writer->L, "SetToLuaFile");
		//char outputString[80];
		//sprintf(outputString, getSPrintFArgument<T>(), identifier.c_str(), value);
		//lua_pushstring(write->L, outputString);
		//Console::Warn << "Writing: " << outputString << '\n';

		//// Push bOverwrite == 2 into the lua state since we are overwriting a value
		//lua_pushinteger(pWriteLuaState, 2);
		//char stringToReplace[80];
		//sprintf(stringToReplace, getSPrintFArgument<T>(),
		//	identifier.c_str(),
		//	get<T>(variableName));
		//lua_pushstring(pWriteLuaState, stringToReplace);
		//std::cout << "This is the old string: " << stringToReplace << std::endl;

		//// Do a lua call with debugging info returned
		//if (lua_pcall(pWriteLuaState, 3, 0, 0))
		//	std::cout << "Unable to save to Lua File: " << lua_tostring(pWriteLuaState, -1) << std::endl;
		//else
		//	std::cout << "Saved to Lua File: " << outputString << std::endl;

		//// Reload the pLuaState with the file's content since we had made changes to the file
		//if (luaL_loadfile(pLuaState, "LUAScripts//DM2240.lua") || lua_pcall(pLuaState, 0, 0, 0))
		//{
		//	std::cout << "Error: script not loaded (" << pLuaStateFilepath << std::endl;
		//	pLuaState = nullptr;
		//	std::cout << "Unable to reload pLuaState" << std::endl;
		//}


		//return true;
	}

	Clean();
	return result;
}

template<typename T>
std::vector<T> LuaScript::GetVector(const std::string & identifer) {
	std::vector<T> v;
	LuaGetToStack(identifer.c_str());

	if (lua_isnil(L, -1)) { // array is not found
		return std::vector<T>();
	}

	lua_pushnil(L);
	while (lua_next(L, -2)) {
		v.push_back((T)lua_tonumber(L, -1));
		lua_pop(L, 1);
	}

	Clean();
	return v;
}

//template<typename T>
//T LuaScript::LuaGet(const std::string & identifier)

template<>
inline std::string LuaScript::LuaGetDefault<std::string>() {
	return "null";
}

template<>
inline bool LuaScript::LuaGet<bool>(const std::string & identifier) {
	return (bool)lua_toboolean(L, -1);
}

template<>
inline int LuaScript::LuaGet<int>(const std::string & identifier) {
	if (!lua_isnumber(L, -1)) {
		PrintError(identifier, "not a number");
		return LuaGetDefault<int>();
	}

	return (int)lua_tonumber(L, -1);
}

template<>
inline float LuaScript::LuaGet<float>(const std::string & identifier) {
	if (!lua_isnumber(L, -1)) {
		PrintError(identifier, "not a number");
		return LuaGetDefault<float>();
	}

	return (float)lua_tonumber(L, -1);
}

template<>
inline std::string LuaScript::LuaGet<std::string>(const std::string & identifier) {
	std::string result = LuaGetDefault<std::string>();

	if (lua_isstring(L, -1)) {
		result = std::string(lua_tostring(L, -1));
	} else {
		PrintError(identifier, "not a number");
	}

	return result;
}

template<>
inline bool LuaScript::LuaSet<bool>(const std::string & identifier, const bool& value) {
	lua_pushboolean(L, value);
	return true;
}

template<>
inline bool LuaScript::LuaSet<int>(const std::string & identifier, const int& value) {
	if (!lua_isnumber(L, -1)) {
		PrintError(identifier, "not a number");
		return false;
	}

	lua_pushnumber(L, value);
	return true;
}

template<>
inline bool LuaScript::LuaSet<float>(const std::string & identifier, const float& value) {
	if (!lua_isnumber(L, -1)) {
		PrintError(identifier, "not a number");
		return false;
	}

	lua_pushnumber(L, value);
	return true;
}

template<>
inline bool LuaScript::LuaSet<std::string>(const std::string & identifier, const std::string& value) {
	std::string result = LuaGetDefault<std::string>();

	if (!lua_isstring(L, -1)) {
		PrintError(identifier, "not a number");
		return false;
	} 

	lua_pushstring(L, identifier.c_str());

	return true;
}

#endif
