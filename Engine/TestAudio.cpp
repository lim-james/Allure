#include "TestAudio.h"

#include "InputEvents.h"
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void TestAudio::Awake() {
	source = GetComponent<AudioSource>();
	EventsManager::Get()->Subscribe("KEY_INPUT", &TestAudio::KeyHandler, this);
}

void TestAudio::KeyHandler(Events::Event * event) {
	const auto input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_PRESS) {

		switch (input->key) {
		case GLFW_KEY_L:
			source->loop = !source->loop;
			break;
		case GLFW_KEY_M:
			source->mute = !source->mute;
			break;
		case GLFW_KEY_EQUAL:
			source->volume += 0.1f;
			break;
		case GLFW_KEY_MINUS:
			source->volume -= 0.1f;
			break;
		case GLFW_KEY_H:
			source->stereoPan = -1.f;
			break;
		case GLFW_KEY_J:
			source->stereoPan = 0.f;
			break;
		case GLFW_KEY_K:
			source->stereoPan = 1.f;
			break;
		case GLFW_KEY_2:
			source->spatialBlend = 0.f;
			break;
		case GLFW_KEY_3:
			source->spatialBlend = 1.f;
			break;
		case GLFW_KEY_SPACE:
			source->Play();
			break;
		case GLFW_KEY_ENTER:
			if (source->IsPaused())
				source->UnPause();
			else 
				source->Pause();
			break;
		case GLFW_KEY_BACKSPACE:
			source->Stop();
			break;
		default:
			break;
		}
	}
}
