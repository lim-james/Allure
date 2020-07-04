#include "SFXEmitter.h"

#include "AudioSource.h"
#include "AudioSelfDestruct.h"	

Transform * SFXEmitter::Create() {
	const unsigned entity = entities->Create();

	AudioSource* const audio = entities->AddComponent<AudioSource>(entity);
	audio->SetActive(true);
	audio->volume = 0.f;

	AudioSelfDestruct* const destruct = entities->AddComponent<AudioSelfDestruct>(entity);
	destruct->SetActive(true);

	return entities->GetComponent<Transform>(entity);
}
