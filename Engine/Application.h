#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "InputController.h"
#include "SceneManager.h"

#include <Timer/Timer.h>
#include <Events/Event.h>

#include <map>

class Application {

#if _DEBUG
	std::map<std::string, Timer> timers;
#endif

	Timer timer;
	float bt;
	
	float freezeTime;

	Window* context;
	InputController inputController;
	SceneManager* sceneManager;

public:

	Application();

	void Initialize(int const& width, int const& height, const char* title, bool const& fullscreen);
	void Run();
	void Exit();

private:

#if _DEBUG
	void OnTimerEvent(Events::Event* event);
#endif

	void Step();
	void Freeze(Events::Event* event);

};

#endif
