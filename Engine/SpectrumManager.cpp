#include "SpectrumManager.h"

#include <Math/Random.hpp>

SpectrumManager::SpectrumManager() 
	: bars(nullptr), file(nullptr) {}

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

	source->Play();
}

void SpectrumManager::Update() {
	t += time->dt * source->speed;

	const auto sample = file->Spectrum(t, audioDuration * 0.001f, frequencyBands, startFrequency, endFrequency);

	const float dH = maxHeight - thickness;
	for (unsigned i = 0; i < frequencyBands; ++i) {
		const float a = sample[i];
		bars[i]->scale.y = dH * a + thickness;
	}
}

void SpectrumManager::OnDestroy() {
	delete[] bars;
}
