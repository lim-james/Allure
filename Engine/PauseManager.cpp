#include "PauseManager.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void PauseManager::Unpause() {
	EventsManager::Get()->Trigger("DISMISS_SCENE");
}

void PauseManager::Quit() {
}

void PauseManager::Awake() {
	std::cout << "XXXXX Awake " << this << '\n';
	EventsManager::Get()->Subscribe("KEY_INPUT", &PauseManager::KeyHanlder, this);
}

void PauseManager::KeyHanlder(Events::Event * event) {
	const auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_ESCAPE && input->action == GLFW_PRESS) {
		std::cout << "XXXXX Unpause " << this << '\n';
		Unpause();
	}
}
