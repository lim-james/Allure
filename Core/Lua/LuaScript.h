#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

extern "C" {
#include  "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "../Logger/Logger.h"

#include <string>

class LuaScript {

	lua_State* L;
	int level;

public:

	LuaScript(const std::string& filepath);
	~LuaScript();

	template<typename T>
	T Get(const std::string& identifier);
	template<typename T>
	T LuaGet(const std::string& identifier);

private:

	bool LuaGetToStack(const std::string& identifier);

	template<typename T>
	T LuaGetDefault();

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

	lua_pop(L, level + 1);
	return result;
}

template<typename T>
T LuaScript::LuaGet(const std::string & identifier) {
	return 0;
}

template<>
bool LuaScript::LuaGet(const std::string & identifier) {
	return (bool)lua_toboolean(L, -1);
}

template<>
float LuaScript::LuaGet(const std::string & identifier) {
	if (!lua_isnumber(L, -1)) {
		PrintError(identifier, "not a number");
		return LuaGetDefault<float>();
	}

	return (float)lua_tonumber(L, -1);
}

template<>
int LuaScript::LuaGet(const std::string & identifier) {
	if (!lua_isnumber(L, -1)) {
		PrintError(identifier, "not a number");
		return LuaGetDefault<int>();
	}

	return (int)lua_tonumber(L, -1);
}

template<>
std::string LuaScript::LuaGet(const std::string & identifier) {
	std::string result = LuaGetDefault<std::string>();

	if (lua_isstring(L, -1)) {
		result = std::string(lua_tostring(L, -1));
	} else {
		PrintError(identifier, "not a number");
	}

	return result;
}

template<typename T>
T LuaScript::LuaGetDefault() {
	return 0;
}

template<>
std::string LuaScript::LuaGetDefault<std::string>() {
	return "null";
}

#endif
