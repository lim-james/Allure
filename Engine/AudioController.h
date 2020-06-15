#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "Script.h"

#include "SpriteRender.h"
#include "AudioSource.h"

#include "BackgroundMaterial.h"

#include <Audio/AudioFile.h>
#include <Events/Event.h>	

struct AudioController : Script {

	Material::Background* material;

	unsigned startFrequency;
	unsigned endFrequency;
	unsigned frequencyBands;

	float audioDuration;

	float maxHeight;
	float* meterHeight;

private:

	float t;
	float duration;

	SpriteRender* render;
	AudioSource* audio;

	AudioFile<int16_t>* file;

	void Awake() override;
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void Stop() override;

	void KeyHandler(Events::Event* event);

};

#endif
