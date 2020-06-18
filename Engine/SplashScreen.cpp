#include "SplashScreen.h"

#include "Text.h"
#include "AnimationSystem.h"
// post vfx
#include "Bloom.h"
// utilities
#include "MainGame.h"
#include "LoadFNT.h"

#include <Helpers./ColorHelpers.h>
#include <Events/EventsManager.h>

void SplashScren::Awake() {
	Scene::Awake();
	systems->Subscribe<AnimationSystem>(1);
}

void SplashScren::Create() {
	Scene::Create();

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

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
		text->text = "TEMPO";
		text->color = vec4f(1.f, 1.f, 1.f, 0.f);
		text->color.a = 0.f;
		text->characterSpacing = 3.f;
		text->scale = 0.75f;

		Animation* const animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &text->color.a, 2.f);
		animation->Queue(AnimationBase(false, 1.f, 1.f, Handler<void, void>([]() {
			EventsManager::Get()->Trigger("PRESENT_SCENE", new Events::PresentScene(new MainGame));
		})), &text->color.a, 0.f);
	}
}
