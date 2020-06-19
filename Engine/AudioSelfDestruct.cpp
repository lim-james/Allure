#include "AudioSelfDestruct.h"

void AudioSelfDestruct::Awake() {
	source = GetComponent<AudioSource>();
}

void AudioSelfDestruct::Start() {
	t = 0.f;
}

void AudioSelfDestruct::Update() {
	if (source->IsPaused()) {
		t += time->dt;
		if (t > source->GetDuration()) {
			entities->Destroy(entity);
		}
	}
}
