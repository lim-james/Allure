#include "MapCreator.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void MapCreator::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &MapCreator::KeyEvents, this);
}

void MapCreator::Update() {
	if (source->IsPaused()) return;
	map.Update(time->dt * source->speed);
}

void MapCreator::KeyEvents(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	
	if (input->action == GLFW_PRESS) {
		if (input->key == GLFW_KEY_Z || input->key == GLFW_KEY_X) {
			map.Push();
		} else if (input->key == GLFW_KEY_SPACE) {
			if (source->IsPaused()) {
				source->Play();
			} else {
				source->Stop();
				map.Save(savePath);
			}
		}
	}
}
