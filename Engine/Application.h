#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "InputController.h"
#include "SceneManager.h"

#include <Timer/Timer.h>
#include <Events/Event.h>

#include <map>

class Application {

	const unsigned FPS = 60;
	const unsigned FRAMERATE_MS = 1000 / FPS;
	const float FRAMERATE = 1.f / static_cast<float>(FPS);

#if _DEBUG
	std::map<std::string, Timer> timers;
#endif

	Timer timer;

	Window* context;
	InputController inputController;
	SceneManager* sceneManager;

public:

	Application();

	void Initialize(const int& width, const int& height, const char* title, const bool& fullscreen);
	void Run();
	void Exit();

private:

	void OnEvent(Events::Event* event);

#if _DEBUG
	void OnTimerEvent(Events::Event* event);
#endif

};

#endif
