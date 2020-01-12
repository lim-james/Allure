#include "Three.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void Three::Awake() {
	Scene::Awake();
}

void Three::InputHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	if (input->action == GLFW_RELEASE) {
		if (input->key == GLFW_KEY_1) {
			Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::PresentScene("ONE"));
		} else if (input->key == GLFW_KEY_2) {
			Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::PresentScene("TWO"));
		} else if (input->key == GLFW_KEY_3) {
			Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::PresentScene("THREE"));
		} else if (input->key == GLFW_KEY_SPACE) {
			Console::Warn << "Printing from " << this << '\n';
		}
	}
}
