#include "AudioControlller.h"

void AudioController::FadeIn(std::string const & path) {
	source->audioClip = path;
	source->volume = 0.f;
	source->Play();

	state = -1;
}

void AudioController::FadeOut() {
	state = 1;
}

void AudioController::Awake() {
	fadeSpeed = 1.f;
	source = GetComponent<AudioSource>();
}

void AudioController::Update() {
	switch (state) {
	case -1:
		source->volume += time->dt * fadeSpeed;
		if (source->volume >= 1.f) {
			source->volume = 1.f;
			state = 0;
		}
		break;
	case 1:
		source->volume -= time->dt * fadeSpeed;
		if (source->volume <= 0.f) {
			source->Stop();
			entities->Destroy(entity);
		}
		break;
	default:
		break;
	}
}
