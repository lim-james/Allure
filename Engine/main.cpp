//extern "C" {
//	_declspec(dllexport) unsigned NvOptimusEnablement = 0x00000001;
//}

#include "Application.h"

//#include <Encoding/AON.hpp>

int main() { 

	//AON notation;

	//notation.Set("Size", vec2f(1600, 900));
	//Debug::Warn << notation.Get<vec2f>("Size") << '\n';

	Application app;

	app.Initialize(1600, 900, "Allure 2D", false);
	app.Run();
	app.Exit();

	return 0;
}