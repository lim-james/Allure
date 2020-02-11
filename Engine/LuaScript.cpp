#include "LuaScript.h"

#include <Helpers/FileHelpers.h>

LuaScript::LuaScript(const std::string & filepath) : level(0) {
	L = luaL_newstate();

	//Console::Warn << Helpers::ReadFile(filepath) << '\n';

	if (luaL_loadfile(L, filepath.c_str()) || lua_pcall(L, 0, 0, 0)) {
		Console::Error << "Script not loaded: \"" << filepath << "\"\n";
		L = nullptr;
	}
}

LuaScript::~LuaScript() {
	if (L) lua_close(L);
}

void LuaScript::Clean() {
	int n = lua_gettop(L);
	lua_pop(L, n);
}

std::vector<std::string> LuaScript::GetKeys(const std::string & identifier) {
	std::string code =
		"function getKeys(identifier) "
		"s = \"\""
		"for k, v in pairs(_G[identifier]) do "
		"    s = s..k..\",\" "
		"    end "
		"return s "
		"end"; // function for getting table keys

	luaL_loadstring(L, code.c_str()); // execute code
	lua_pcall(L, 0, 0, 0);
	lua_getglobal(L, "getKeys"); // get function
	lua_pushstring(L, identifier.c_str());
	lua_pcall(L, 1, 1, 0); // execute function

	std::string test = lua_tostring(L, -1);
	std::vector<std::string> strings;
	std::string temp = "";
	std::cout << "TEMP:" << test << std::endl;

	for (unsigned int i = 0; i < test.size(); i++) {
		if (test.at(i) != ',') {
			temp += test.at(i);
		} else {
			strings.push_back(temp);
			temp = "";
		}
	}

	Clean();
	return strings;
}

bool LuaScript::LuaGetToStack(const std::string & identifier) {
	level = 0;
	std::string var = "";
	for (unsigned int i = 0; i < identifier.size(); i++) {
		if (identifier.at(i) == '.') {
			if (level == 0) {
				lua_getglobal(L, var.c_str());
			} else {
				lua_getfield(L, -1, var.c_str());
			}

			if (lua_isnil(L, -1)) {
				PrintError(identifier, "not defined");
				return false;
			} else {
				var = "";
				++level;
			}
		} else {
			var += identifier.at(i);
		}
	}

	if (level == 0) {
		lua_getglobal(L, var.c_str());
	} else {
		lua_getfield(L, -1, var.c_str());
	}

	if (lua_isnil(L, -1)) {
		PrintError(identifier, "not defined");
		return false;
	}

	return true;
}

void LuaScript::PrintError(const std::string & identifier, const std::string & reason) const {
	Console::Error << "\"" << identifier << "\": " << reason << '\n';
}
