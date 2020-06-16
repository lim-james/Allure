#include "SpectrumManager.h"

#include "InputEvents.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

SpectrumManager::SpectrumManager() 
	: bars(nullptr), file(nullptr) {}

void SpectrumManager::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &SpectrumManager::KeyHandler, this);
}

void SpectrumManager::Start() {
	t = 0.f;

	if (!file) {
		file = new AudioFile<int16_t>;
		file->Open(source->audioClip);
	}

	if (!bars) {
		bars = new Transform*[frequencyBands];

		const float halfW = width * 0.5f;
		const float bands = static_cast<float>(frequencyBands);

		for (float i = 0; i < bands; ++i) {
			Transform* const t = barPrefab->Create();
			t->scale.xy = thickness;
			t->translation.x = i / (bands - 1) * width - halfW;
			bars[static_cast<unsigned>(i)] = t;
		}
	}
}

void SpectrumManager::Update() {
	if (source->IsPaused()) return;

	t += time->dt * source->speed;

	const auto sample = file->Spectrum(t, audioDuration * 0.001f, frequencyBands, startFrequency, endFrequency);

	const float halfW = width * 0.5f;

	const float dH = maxHeight - thickness;
	for (unsigned i = 0; i < frequencyBands; ++i) {
		const vec2f a = sample[i];
		const float height = dH * a.y + thickness;
		bars[i]->translation.x = a.x * width - halfW;
		bars[i]->translation.y = height * 0.5f;
		bars[i]->scale.y = height;
	}
}

void SpectrumManager::OnDestroy() {
	delete[] bars;
}

void SpectrumManager::KeyHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_PRESS) {
		t = 0.f;
		source->Play();

		//const auto sample = file->Spectrum(t, audioDuration * 0.001f, frequencyBands, startFrequency, endFrequency);

		//const float dH = maxHeight - thickness;
		//for (unsigned i = 0; i < frequencyBands; ++i) {
		//	const vec2d a = sample[i];
		//	bars[i]->translation.x = a.x * width - width * .5f;
		//	bars[i]->scale.y = dH * a.y + thickness;
		//}
	}
}
