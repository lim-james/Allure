//extern "C" {
//	_declspec(dllexport) unsigned NvOptimusEnablement = 0x00000001;
//}

#include <Loaders/LoadWAV.hpp>

#include "Application.h"

int main() { 

	int16_t* data = nullptr;

	size_t size = Load::WAV("Files/Media/LOUD - Thoughts.wav", &data, 22.f, 0.001f);
	//auto result = Load::WAV("Files/Media/hit.wav");

	for (unsigned i = 0; i < size; i += 2)
		std::cout << (float)data[i] / 32768.f << '\t' << (float)data[i + 1] / 32768.f << '\n';

	return 0;

	Application app;

	app.Initialize(1600, 900, "Allure", false);
	app.Run();
	app.Exit();

	return 0;
}