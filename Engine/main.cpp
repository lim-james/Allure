//extern "C" {
//	_declspec(dllexport) unsigned NvOptimusEnablement = 0x00000001;
//}

//#include <Math/Vectors.hpp>

#include "Application.h"

#include "LuaScript.h"

int main() { 
	Application app;

	LuaScript* script = new LuaScript("Files/Scripts/Config.lua");

	app.Initialize(
		script->Get<int>("window.size.width"), 
		script->Get<int>("window.size.height"), 
		script->Get<std::string>("window.title").c_str(), 
		script->Get<bool>("window.fullscreen")
	);

	delete script;

	app.Run();
	app.Exit();

	return 0;
}