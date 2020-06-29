#include "Application.h"
#include "ApplicationProperties.h"

#include "TitleScene.h"
#include "SampleScene.h"
#include "Stress3D.h"
#include "SplashScene.h"
#include "MainGame.h"
#include "MenuScene.h"
#include "PauseScene.h"
#include "ScoreScene.h"
#include "AudioSpectrumScene.h"

#include "InputEvents.h"
#include "LoadFNT.h"
#include "LoadModel.h"
// external
#include <Events/EventsManager.h>
#include <Logger/Logger.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Application::Application()
	: context(nullptr) {}

void Application::Initialize(int const& width, int const& height, const char* title, bool const& fullscreen) {
	// initialize GLFW
	if (!glfwInit()) {
		Debug::Error << "GLFW INITIALISATION FAILED.\n";
		return;
	}

	context = new Window(width, height, title, fullscreen);
	inputController.Initialize(context->Get());

	// initialize GLEW
	glewExperimental = true;
	const GLenum err = glewInit();
	if (err != GLEW_OK) {
		Debug::Error << glewGetErrorString(err) << '\n';
	}

	auto em = EventsManager::Get();

#if _DEBUG
	em->Subscribe("TIMER_START", &Application::OnTimerEvent, this);
	em->Subscribe("TIMER_STOP", &Application::OnTimerEvent, this);
#endif

	em->Subscribe("STEP", &Application::Step, this);
	em->Subscribe("FREEZE", &Application::Freeze, this);

	// turn off vsync
	//glfwSwapInterval(0);

	sceneManager = new SceneManager;
	sceneManager->Present(new SplashScene);

	context->BroadcastSize();
	em->TriggerQueued();
}

void Application::Run() {
	timer.Start();

	auto em = EventsManager::Get();

	em->Trigger("CURSOR_SENSITIVITY", new Events::AnyType<float>(0.1f));

	bt = 0.f;
	while (!context->ShouldClose()) {
		//em->Trigger("STEP");
		Step();
	}
}

void Application::Exit() {
#if _DEBUG
	timers.clear();
#endif

	delete context;
	delete sceneManager;

	Load::ClearFontCache();
	Load::ClearModelCache();

	EventsManager::Destroy();
}

#if _DEBUG
void Application::OnTimerEvent(Events::Event* event) {
	const auto timer = static_cast<Events::String*>(event);

	if (event->name == "TIMER_START") {
		timers[timer->data].Start();
	} else if (event->name == "TIMER_STOP") {
		timers[timer->data].Update();
		Debug::Log << "[TIMER] " << timer->data << " : " << timers[timer->data].GetElapsedTime() << '\n';
	}
}
#endif

void Application::Step() {
	const float et = static_cast<float>(timer.GetElapsedTime());
	float dt = static_cast<float>(timer.GetDeltaTime());

	if (freezeTime > 0) {
		freezeTime -= dt;
		if (freezeTime >= 0.f) {
			ft += dt;
			timer.Update();
			return;
		} else {
			dt = -freezeTime + ft;
			ft = 0.f;
		}
	}

	//context->SetTitle(std::to_string((int)(1.0f / dt)).c_str());

	Scene* const current = sceneManager->GetSource();

	bt += dt;

	if (bt >= FIXED_TIME_STEP) {
		do {
			current->FixedUpdate(FIXED_TIME_STEP);
			bt -= FIXED_TIME_STEP;
		} while (bt >= FIXED_TIME_STEP);
	}

	current->Update(dt);

	glfwPollEvents();

	EventsManager::Get()->TriggerQueued();

	sceneManager->Segue();
	context->UpdateFrame(dt);
	timer.Update();
}

void Application::Freeze(Events::Event * event) {
	freezeTime = static_cast<Events::AnyType<float>*>(event)->data;
}
