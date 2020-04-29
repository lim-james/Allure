#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include "Component.h"

struct AudioSource : Component {

	std::string audioClip;

	bool mute;
	bool loop;

	float volume;
	float speed;
	float stereoPan;
	float spatialBlend;

	float dopplerLevel;
	float spread;
	float minDistance;
	float maxDistance;

	AudioSource();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	void Play();
	void Pause();
	void Stop();

};

#endif
