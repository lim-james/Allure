#include "AudioSource.h"

#include <Events/EventsManager.h>

AudioSource::AudioSource() 
	: audioClip("")
	, mute(false)
	, loop(false)

	, volume(1.f)
	, speed(1.f)
	, stereoPan(0.f)
	, spatialBlend(0.f) 

	//, dopplerLevel(1.f)
	, minDistance(1.f)
	, maxDistance(10.f) {
}

void AudioSource::Initialize() {
	audioClip = "";
	mute = false;
	loop = false;

	volume = 1.f;
	speed = 1.f;
	stereoPan = 0.f;
	spatialBlend = 0.f;

	//dopplerLevel = 1.f;
	minDistance = 1.f;
	maxDistance = 500.f;
}

Component * AudioSource::Clone() const {
	return new AudioSource(*this);
}

void AudioSource::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("AUDIO_SOURCE_ACTIVE", new Events::AnyType<AudioSource*>(this));
}

void AudioSource::Play() {
	if (IsActive()) {
		EventsManager::Get()->Trigger("AUDIO_SOURCE_PLAY", new Events::AnyType<AudioSource*>(this));
	}
}

bool AudioSource::IsPaused() const {
	return isPaused;
}

void AudioSource::Pause() {
	if (IsActive()) {
		EventsManager::Get()->Trigger("AUDIO_SOURCE_PAUSE", new Events::AnyType<AudioSource*>(this));
	}
}

void AudioSource::UnPause() {
	if (IsActive()) {
		EventsManager::Get()->Trigger("AUDIO_SOURCE_UNPAUSE", new Events::AnyType<AudioSource*>(this));
	}
}

void AudioSource::Stop() {
	if (IsActive()) {
		EventsManager::Get()->Trigger("AUDIO_SOURCE_STOP", new Events::AnyType<AudioSource*>(this));
	}
}

