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

//template<>
//std::string LuaScript::LuaGetDefault() {
//	return "null";
//}


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
