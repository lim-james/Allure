#include "AudioSystem.h"

#include "Transform.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>

int AudioSystem::systemCount = 0;
irrklang::ISoundEngine* AudioSystem::engine = nullptr;

AudioSystem::Sound::Sound() 
	: sound2d(nullptr)
	, sound3d(nullptr) {}

AudioSystem::AudioSystem() {
	if (engine == nullptr)
		engine = irrklang::createIrrKlangDevice();

	++systemCount;
}

AudioSystem::~AudioSystem() {
	for (auto& soundPair : queued) {
		for (Sound& sound : soundPair.second) { 
			sound.sound2d->drop();
			if (sound.sound3d) sound.sound3d->drop();
		}
	}

	for (auto& soundPair : playing) {
		Sound& sound = soundPair.second;
		sound.sound2d->drop();
		if (sound.sound3d) sound.sound3d->drop();
	}

	--systemCount;
	if (systemCount <= 0 && engine != nullptr) {
		engine->drop();
		engine = nullptr;
	}
}

void AudioSystem::Initialize() {
	EventsManager* const em = EventsManager::Get();

	em->Subscribe("AUDIO_LISTENER_ACTIVE", &AudioSystem::ListenerActiveHandler, this);
	em->Subscribe("AUDIO_SOURCE_ACTIVE", &AudioSystem::SourceActiveHandler, this);
	em->Subscribe("AUDIO_SOURCE_PLAY", &AudioSystem::PlayHandler, this);
	em->Subscribe("AUDIO_SOURCE_PAUSE", &AudioSystem::PauseHandler, this);
	em->Subscribe("AUDIO_SOURCE_STOP", &AudioSystem::StopHandler, this);
}

void AudioSystem::Update(float const & dt) {
	for (auto i = playing.cbegin(); i != playing.cend();) {
		AudioSource* const source = i->first;
		const Sound sound = i->second;

		if (sound.sound2d->isFinished()) {
			playing.erase(i++);
		} else {
			++i;
		}
	}

	for (auto& soundPair : queued) {
		AudioSource* const source = soundPair.first;

		// check if anything is playing for source
		if (playing.find(source) == playing.end()) {
			// get all queued
			auto& sounds = soundPair.second;
			if (sounds.empty()) continue;

			// assign first queued
			playing[source] = sounds[0];
			// play sounds
			sounds[0].sound2d->setIsPaused(false);
			if (sounds[0].sound3d) 
				sounds[0].sound3d->setIsPaused(false);
			// remove from queue
			sounds.erase(sounds.begin());
		}
	}

	for (AudioListener* const listener : listeners) {
		Transform* const listenerTransform = entities->GetComponent<Transform>(listener->entity);
		const vec3f position = listenerTransform->GetWorldTranslation();
		const vec3f direction = listenerTransform->GetWorldFront();

		engine->setListenerPosition(
			irrklang::vec3df(position.x, position.y, position.z),
			irrklang::vec3df(direction.x, direction.y, direction.z)
		);
	}
}

void AudioSystem::ListenerActiveHandler(Events::Event * event) {
	AudioListener* const c = static_cast<Events::AnyType<AudioListener*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(listeners, c);
	} else {
		Helpers::Remove(listeners, c);
	}
}

void AudioSystem::SourceActiveHandler(Events::Event * event) {
	AudioSource* const c = static_cast<Events::AnyType<AudioSource*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(sources, c);
	} else {
		Helpers::Remove(sources, c);
	}
}

void AudioSystem::PlayHandler(Events::Event * event) {
	AudioSource* const source = static_cast<Events::AnyType<AudioSource*>*>(event)->data;

	Transform* const transform = entities->GetComponent<Transform>(source->entity);
	const vec3f position = transform->GetWorldTranslation();

	const float volume2D = source->volume * (1.f - source->spatialBlend);
	const float volume3D = source->volume * source->spatialBlend;

	Sound sound;

	sound.sound2d = engine->play2D(source->audioClip.c_str(), source->loop, true, true);
	sound.sound2d->setVolume(volume2D);
	sound.sound2d->setPlaybackSpeed(source->speed);
	sound.sound2d->setPan(source->stereoPan);

	if (volume3D > 0.f) {
		const irrklang::vec3df iSoundPosition = irrklang::vec3df(position.x, position.y, position.z);
		sound.sound3d = engine->play3D(source->audioClip.c_str(), iSoundPosition, source->loop, true, true);

		sound.sound3d->setVolume(volume3D);
		sound.sound3d->setPlaybackSpeed(source->speed);
		sound.sound3d->setPan(source->stereoPan);
		sound.sound3d->setMinDistance(source->minDistance);
		sound.sound3d->setMaxDistance(source->maxDistance);
	}

	queued[source].push_back(sound);
}

void AudioSystem::PauseHandler(Events::Event * event) {
	AudioSource* const source = static_cast<Events::AnyType<AudioSource*>*>(event)->data;
	Sound& sound = playing[source];

	sound.sound2d->setIsPaused(true);
	if (sound.sound3d) sound.sound3d->setIsPaused(true);
}

void AudioSystem::StopHandler(Events::Event * event) {
	AudioSource* const source = static_cast<Events::AnyType<AudioSource*>*>(event)->data;
	Sound& sound = playing[source];

	sound.sound2d->stop();
	sound.sound2d->drop();

	if (sound.sound3d) {
		sound.sound3d->stop();
		sound.sound3d->drop();
	}

	playing.erase(source);
}
