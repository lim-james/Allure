#include "MenuScene.h"

#include "Camera.h"
#include "Text.h"
#include "SpriteRender.h"
#include "AnimationSystem.h"
#include "AudioSystem.h"
// scripts
#include "BubbleManager.h"
// post vfx
#include "Bloom.h"
// utilities
#include "LoadFNT.h"
#include "LoadTexture.h"

#include <Helpers./ColorHelpers.h>
#include <Events/EventsManager.h>

void MenuScene::Awake() {
	Scene::Awake();

	systems->Subscribe<AudioSystem>(0);
	systems->Subscribe<AnimationSystem>(1);

	spectrumBubble = new Material::SpectrumBubble;
	spectrumBubble->minRadius = 0.5f;
	spectrumBubble->maxValue = 1.f;
	spectrumBubble->unit = 1.f;
	//spectrumBubble->multiplier = 1.5f;
	spectrumBubble->multiplier = 10.f;

	audioPlayer = new AudioPlayer();
	audioPlayer->Initialize(entities);
}

void MenuScene::Create() {
	Scene::Create();

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(30.f);
	camera->projection = ORTHOGRAPHIC;

	// post processing volume
	{
		const unsigned volume = entities->Create();
		Bloom* bloom = entities->AddComponent<Bloom>(volume);
		bloom->SetActive(true);
		bloom->unit = 2.f;
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, -5.f));
		transform->SetScale(vec3f(160.0f, 90.0f, 1.0f));
		transform->SetDynamic(false);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(Load::TGA("Files/Textures/tile.tga"));
		//render->SetCellRect(0, 0, 32, 18);
		render->SetCellRect(0, 0, 12, 6.75);
		render->tint.a = 0.f;

		Animation* const animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 2.f), &render->tint.a, 0.15f);
		animation->Queue(AnimationBase(true, 10.f), &render->uvRect.xy, vec2f(1.f));
	}

	{
		const unsigned entity = entities->Create();
	
		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(60.f);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(spectrumBubble);
		render->tint.a = 0.f;

		//source->audioClip = "Files/Media/128C.wav";
		//source->audioClip = "Files/Media/LOUD - Thoughts.wav";
		//source->audioClip = "Files/Media/IceFlow.wav";
		//source->speed = 1.5f;

		Animation* const animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &render->tint.a, 1.f);

		BubbleManager* const bubble = entities->AddComponent<BubbleManager>(entity);
		bubble->SetActive(true);

		bubble->material = spectrumBubble;
		bubble->startFrequency = 20;
		bubble->endFrequency = 2000;
		bubble->audioDuration = 90.f;

		bubble->audioPrefab = audioPlayer;
		bubble->AddSong("Files/Media/LOUD - Thoughts.wav");
		bubble->AddSong("Files/Media/128C.wav");
		bubble->AddSong("Files/Media/IceFlow.wav");
		bubble->AddSong("Files/Media/ass.wav");
		bubble->AddSong("Files/Media/KraftyKuts.wav");
	}
}

void MenuScene::Destroy() {
	Scene::Destroy();

	delete spectrumBubble;
	delete audioPlayer;
}
