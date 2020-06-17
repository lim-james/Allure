#include "SplashScreen.h"

#include "Text.h"
#include "SpriteRender.h"
#include "AnimationSystem.h"
// post vfx
#include "Bloom.h"

#include "LoadFNT.h"

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
		entities->AddComponent<Bloom>(volume)->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->text = "TEMPO";
		//text->color = vec4f(2.f, 2.f, 2.f, 0.f);
		text->color.a = 0.f;
		text->characterSpacing = 3.f;
		text->scale = 0.75f;

		Animation* const animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &text->color.a, 1.f);
	}
}
