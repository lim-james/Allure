//extern "C" {
//	_declspec(dllexport) unsigned NvOptimusEnablement = 0x00000001;
//}

#include "Application.h"

int main() { 

	Application app;

	app.Initialize(1600, 900, "Allure", false);
	app.Run();
	app.Exit();

	return 0;
}