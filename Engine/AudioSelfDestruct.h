#ifndef AUDIO_SELF_DESTRUCT_H
#define AUDIO_SELF_DESTRUCT_H

#include "Script.h"
#include "AudioSource.h"

struct AudioSelfDestruct : Script {

private:

	float t;
	AudioSource* source;

	void Awake() override;
	void Start() override;
	void Update() override;

};

#endif
