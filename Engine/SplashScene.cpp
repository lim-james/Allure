#include "SplashScene.h"

#include "Text.h"
#include "AnimationSystem.h"
// post vfx
#include "Bloom.h"
// utilities
#include "MenuScene.h"
#include "LoadFNT.h"

#include <Helpers./ColorHelpers.h>
#include <Events/EventsManager.h>

void SplashScene::Awake() {
	Scene::Awake();
	systems->Subscribe<AnimationSystem>(1);
}

void SplashScene::Create() {
	Scene::Create();

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(15.f);
	camera->projection = ORTHOGRAPHIC;

	// post processing volume
	{
		const unsigned volume = entities->Create();
		Bloom* bloom = entities->AddComponent<Bloom>(volume);
		bloom->SetActive(true);
		bloom->unit = 1.f;
	}

	{
		const unsigned entity = entities->Create();

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->text = "Use headphones for best experience.";
		text->color = vec4f(1.f, 1.f, 1.f, 0.f);
		text->scale = 1.f;
		text->offset.y = -0.25f;

		Animator* const animation = entities->AddComponent<Animator>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &text->offset.y, 0.f);
		animation->Queue(AnimationBase(false, 1.f, 3.f, Handler<void, void>([text]() {
			text->offset = -0.25f;
		})), &text->offset.y, 0.25f);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &text->offset.y, 0.f);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &text->offset.y, 0.25f);

		animation->Queue(AnimationBase(false, 1.f, 1.f), &text->color.a, 1.f);
		animation->Queue(AnimationBase(false, 1.f, 3.f, Handler<void, void>([text]() {
			text->characterSpacing = 3.f;
			text->text = "TEMPO";
			text->scale = 1.5f;
		})), &text->color.a, 0.f);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &text->color.a, 2.f);
		animation->Queue(AnimationBase(false, 1.f, 1.f, Handler<void, void>([]() {
			EventsManager::Get()->Trigger("PRESENT_SCENE", new Events::PresentScene(new MenuScene));
		})), &text->color.a, 0.f);
	}
}
