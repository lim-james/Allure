#include "MenuScene.h"

#include "Camera.h"
#include "Text.h"
#include "SpriteRender.h"
#include "AnimationSystem.h"
#include "AudioSystem.h"
#include "LayoutSystem.h"
// scripts
#include "MenuCamera.h"
#include "BubbleManager.h"
#include "MenuManager.h"
// post vfx
#include "Bloom.h"
// utilities
#include "Layers.h"
#include "LoadFNT.h"
#include "LoadTexture.h"

#include <Helpers./ColorHelpers.h>
#include <Events/EventsManager.h>

void MenuScene::Awake() {
	Scene::Awake();

	systems->Subscribe<AudioSystem>(0);
	systems->Subscribe<AnimationSystem>(1);
	systems->Subscribe<LayoutSystem>(1);

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

	{
		Transform* const transform = entities->GetComponent<Transform>(mainCamera);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, 20.f));

		Camera* const camera = entities->GetComponent<Camera>(mainCamera);
		camera->cullingMask = DEFAULT;
	}

	// ui camera
	Camera* uiCamera = nullptr;
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, 0.f, 10.f));

		uiCamera = entities->AddComponent<Camera>(entity);
		uiCamera->SetActive(true);
		uiCamera->SetSize(15.f);
		uiCamera->SetDepth(1);
		uiCamera->shouldClear = false;
		uiCamera->projection = ORTHOGRAPHIC;
		uiCamera->cullingMask = UI;

		MenuCamera* const control = entities->AddComponent<MenuCamera>(entity);
		control->SetActive(true);
		control->speed = 1.f;
		control->range = 2.f;
	}

	// post processing volume
	{
		const unsigned volume = entities->Create();
		Bloom* bloom = entities->AddComponent<Bloom>(volume);
		bloom->SetActive(true);
		bloom->unit = 2.f;
	}

	// grid
	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetLocalTranslation(vec3f(0.f, -5.f, -5.f));
		transform->SetLocalRotation(vec3f(130.f, 0.f, 0.f));
		transform->SetScale(vec3f(160.0f, 90.0f, 1.0f));
		transform->SetDynamic(false);

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetSprite(Load::TGA("Files/Textures/tile.tga"));
		render->SetCellRect(0, 0, 32, 18);
		render->tint.a = 0.15f;

		Animation* const animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 2.f), &render->tint.a, 0.15f);
		animation->Queue(AnimationBase(true, 10.f), &render->uvRect.y, 1.f);
	}

	// spectrum bubble
	BubbleManager* bubble = nullptr;
	{
		const unsigned entity = entities->Create();
		entities->SetLayer(entity, UI);
	
		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(30.f);

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint{ RIGHT_ANCHOR, nullptr, RIGHT_ANCHOR, 1.f, 0.f, uiCamera });
		layout->AddConstraint(Constraint{ CENTER_Y_ANCHOR, nullptr, CENTER_Y_ANCHOR, 1.f, 0.f, uiCamera });

		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		render->SetMaterial(spectrumBubble);
		render->tint.a = 0.f;
		render->SetSprite(Load::Texture2D("Files/Textures/rustediron2_basecolor.png"));

		Animation* const animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &render->tint.a, 1.f);

		bubble = entities->AddComponent<BubbleManager>(entity);
		bubble->SetActive(true);
		bubble->material = spectrumBubble;
		bubble->audioPrefab = audioPlayer;
		bubble->startFrequency = 20;
		bubble->endFrequency = 2000;
		bubble->audioDuration = 90.f;
		bubble->minRadius = 0.5f;
		bubble->maxRadius = 0.6f;
	}

	// menu manager
	{
		const unsigned entity = entities->Create();

		MenuManager* manager = entities->AddComponent<MenuManager>(entity);
		manager->SetActive(true);
		manager->bubble = bubble;
		manager->AddSong(SongData{ "LOUD - Thoughts", "Files/Media/LOUD - Thoughts.wav", 150 });
		manager->AddSong(SongData{ "128C", "Files/Media/128C.wav", 128 });
		manager->AddSong(SongData{ "Ice Flow", "Files/Media/IceFlow.wav", 140 });
		manager->AddSong(SongData{ "Running in the 90's", "Files/Media/Running in the 90's.wav", 118 });
	}
}

void MenuScene::Destroy() {
	Scene::Destroy();

	delete spectrumBubble;
	delete audioPlayer;
}
