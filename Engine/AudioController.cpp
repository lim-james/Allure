#include "AudioController.h"

void AudioController::Awake() {
	render = GetComponent<SpriteRender>();
	audio = GetComponent<AudioSource>();
}

void AudioController::Start() {
	t = 0.f;
	audio->Play();

	file = new AudioFile<int16_t>;
	file->Open(audio->audioClip);
}

void AudioController::Update() {
	material->et += time->dt;
}

void AudioController::FixedUpdate() {
	t += time->fixedDt;

	//const int16_t data = file->AverageEnergy(t, 10, 10);
	//const float result = static_cast<float>(data) / 32768.f;

	//render->tint = result;
}

void AudioController::Stop() {
	audio->Pause();
}
