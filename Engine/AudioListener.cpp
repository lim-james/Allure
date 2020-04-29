#include "AudioListener.h"

#include <Events/EventsManager.h>

void AudioListener::Initialize() {}

Component * AudioListener::Clone() const {
	return new AudioListener(*this);
}

void AudioListener::SetActive(bool const & state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("AUDIO_LISTENER_ACTIVE", new Events::AnyType<AudioListener*>(this));
}
