//extern "C" {
//	_declspec(dllexport) unsigned NvOptimusEnablement = 0x00000001;
//}

//#include <Math/Vectors.hpp>

#include "Application.h"

#include <Lua/LuaScript.h>

int main() { 

	LuaScript* script = new LuaScript("Files/Scripts/Config.lua");

	Console::Warn << "Width : " << script->Get<float>("window.size.width") << '\n';
	Console::Warn << "Height: " << script->Get<float>("window.size.height") << '\n';
	Console::Warn << "Title : " << script->Get<std::string>("window.title") << '\n';
	Console::Warn << "Full  : " << script->Get<bool>("window.fullscreen") << '\n';

	delete script;

	while (true) {}

	Application app;

	app.Initialize(1600, 900, "Allure 2D", false);
	app.Run();
	app.Exit();

	return 0;
}