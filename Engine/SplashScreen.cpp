#include "SplashScreen.h"

#include "Text.h"
#include "SpriteRender.h"
#include "AnimationSystem.h"
// post vfx
#include "Bloom.h"

#include "LoadFNT.h"

#include <Helpers./ColorHelpers.h>

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
		//bloom->size = 5;
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

		//SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		//render->SetActive(true);
		////render->tint = vec4f(1.5f);
		//render->tint = Helpers::GetColor("FF073A");
		//render->tint.a = 5.f;

		Animation* const animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
		animation->Queue(AnimationBase(false, 1.f, 1.f), &text->color.a, 2.f);
	}
}
