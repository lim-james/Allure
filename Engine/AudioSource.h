#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include "Component.h"

struct AudioSource : Component {

	using base_type = AudioSource;

	std::string audioClip;

	bool mute;
	bool loop;

	float volume;
	float speed;
	float stereoPan;
	float spatialBlend;

	//float dopplerLevel;
	float minDistance;
	float maxDistance;

	AudioSource();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	void Queue();

	void Play();
	void Stop();

	bool IsPaused() const;
	void Pause();
	void UnPause();

	float GetDuration() const;

	float GetTime() const;
	void SetTime(float const& time);

private:

	bool isPaused;
	float duration;

	float t;

	friend class AudioSystem;

};

#endif
