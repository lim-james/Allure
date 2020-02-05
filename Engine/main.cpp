//extern "C" {
//	_declspec(dllexport) unsigned NvOptimusEnablement = 0x00000001;
//}

#include "Application.h"

int main() {

	Application app;

	app.Initialize(900, 1600, "Flappy Bird", false);
	app.Run();
	app.Exit();

	return 0;
}