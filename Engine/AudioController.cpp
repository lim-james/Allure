#include "AudioController.h"

#include "InputEvents.h"
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void AudioController::Awake() {
	render = GetComponent<SpriteRender>();
	audio = GetComponent<AudioSource>();

	EventsManager::Get()->Subscribe("KEY_INPUT", &AudioController::KeyHandler, this);
}

void AudioController::Start() {
	t = 0.f;
	//audio->Play();

	file = new AudioFile<int16_t>;
	file->Open(audio->audioClip);
	duration = file->GetDuration();
}

void AudioController::Update() {
	if (!audio->IsPaused())
	{
		const float dt = time->dt * audio->speed;
		material->et += dt;
		t += dt;

		while (t > duration) {
			t -= duration;
		}
	}
}

void AudioController::FixedUpdate() {
	if (audio->IsPaused()) return;

	//const auto sample = file->Spectrum(t, audioDuration * 0.001f, frequencyBands, startFrequency, endFrequency);
	float result = 0.f;
	//for (unsigned i = 0; i < frequencyBands; ++i)
		//result = max(result, sample[i]);
	//result = sample[2];

	EventsManager::Get()->Trigger("BEAT_VALUE", new Events::AnyType<float>(result));
	*meterHeight = maxHeight * result;
}

void AudioController::Stop() {
	audio->Pause();
}

void AudioController::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_PRESS) {
		audio->Play();
	}
}
