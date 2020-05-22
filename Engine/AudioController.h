#ifndef AUDIO_CONTROLLER_H
#define AUDIO_CONTROLLER_H

#include "Script.h"

#include "SpriteRender.h"
#include "AudioSource.h"

#include "BackgroundMaterial.h"

#include <Audio/AudioFile.h>

struct AudioController : Script {

	Material::Background* material;

private:

	float t;

	SpriteRender* render;
	AudioSource* audio;

	AudioFile<int16_t>* file;

	void Awake() override;
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void Stop() override;

};

#endif
