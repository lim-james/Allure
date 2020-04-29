#include "Application.h"

#include "TitleScene.h"
#include "SampleScene.h"

#include "InputEvents.h"
#include "LoadFNT.h"
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

	auto em = Events::EventsManager::GetInstance();

#if _DEBUG
	em->Subscribe("TIMER_START", &Application::OnTimerEvent, this);
	em->Subscribe("TIMER_STOP", &Application::OnTimerEvent, this);
#endif

	em->Subscribe("STEP", &Application::Step, this);

	// turn off vsync
	//glfwSwapInterval(0);

	sceneManager = new SceneManager;
	sceneManager->Present(new TitleScene);

	context->BroadcastSize();
	em->TriggerQueued();
}

void Application::Run() {
	timer.Start();

	auto em = Events::EventsManager::GetInstance();

	em->Trigger("CURSOR_SENSITIVITY", new Events::AnyType<float>(0.1f));

	bt = 0.f;
	while (!context->ShouldClose()) {
		em->Trigger("STEP");
	}
}

void Application::Exit() {
#if _DEBUG
	timers.clear();
#endif

	delete context;
	delete sceneManager;

	Load::ClearFontCache();

	Events::EventsManager::Destroy();
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
	const float dt = static_cast<float>(timer.GetDeltaTime());

	//context->SetTitle(std::to_string((int)(1.0f / dt)).c_str());

	auto current = sceneManager->GetSource();

	bt += dt;
	if (bt >= FRAMERATE) {
		current->FixedUpdate(bt);
		bt = 0.f;
	}

	current->Update(dt);

	glfwPollEvents();

	Events::EventsManager::GetInstance()->TriggerQueued();

	sceneManager->Segue();
	context->UpdateFrame(dt);
	timer.Update();
}
