#ifndef SPRECTRUM_MANAGER_H
#define SPRECTRUM_MANAGER_H

#include "Script.h"
#include "Prefab.h"

#include "AudioSource.h"
#include <Audio/AudioFile.h>

struct SpectrumManager : Script {

	AudioSource* source;

	float width;
	unsigned startFrequency;
	unsigned endFrequency;

	float maxHeight;
	float thickness;

	float audioDuration;

	unsigned frequencyBands;
	Prefab* barPrefab;

	SpectrumManager();

private:

	float t;
	Transform** bars;

	AudioFile<int16_t>* file;

	void Start() override;
	void Update() override;
	void OnDestroy() override;

};

#endif
