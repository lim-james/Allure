//extern "C" {
//	_declspec(dllexport) unsigned NvOptimusEnablement = 0x00000001;
//}

#include "Application.h"

int main() { 

	FreeConsole();

	Application app;

	app.Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, IS_FULLSCREEN);
	app.Run();
	app.Exit();

	return 0;
}