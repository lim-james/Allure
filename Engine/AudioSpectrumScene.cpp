#include "AudioSpectrumScene.h"

#include "AudioSystem.h"
#include "SpectrumManager.h"

void AudioSpectrumScene::Awake() {
	Scene::Awake();
	systems->Subscribe<AudioSystem>(0);
}

void AudioSpectrumScene::Create() {
	Scene::Create();

	soundBar = new SoundBar;
	soundBar->Initialize(entities);
	
	{
		const unsigned entity = entities->Create();

		entities->GetComponent<Transform>(entity)->SetDynamic(false);

		AudioSource* const source = entities->AddComponent<AudioSource>(entity);
		source->SetActive(true);
		source->audioClip = "Files/Media/IceFlow.wav";
		source->speed = 1.5f;

		SpectrumManager* const manager = entities->AddComponent<SpectrumManager>(entity);
		manager->SetActive(true);
		manager->source = source;
		manager->width = 15.f;
		manager->startFrequency = 20.f;
		manager->endFrequency = 2000.f;
		manager->audioDuration = 9.f;
		manager->thickness = 0.05f;
		manager->frequencyBands = 128;
		manager->maxHeight = 10.f;
		manager->barPrefab = soundBar;
	}
}

void AudioSpectrumScene::Destroy() {
	delete soundBar;
}
