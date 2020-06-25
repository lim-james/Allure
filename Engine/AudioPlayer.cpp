#include "AudioPlayer.h"

#include "AudioSource.h"
#include "AudioControlller.h"

Transform * AudioPlayer::Create() {
	const unsigned entity = entities->Create();

	AudioSource* const audio = entities->AddComponent<AudioSource>(entity);
	audio->SetActive(true);
	audio->loop = true;
	audio->volume = 0.f;

	AudioController* const controller = entities->AddComponent<AudioController>(entity);
	controller->SetActive(true);
	controller->fadeSpeed = 0.5f;

	return entities->GetComponent<Transform>(entity);
}
