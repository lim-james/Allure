#include "ProcessTest.h"

#include "DotVFX.h"
#include "PopVFX.h"
#include "PaintVFX.h"
#include "Halftone.h"
#include "Pixelated.h"
#include "SpriteRender.h"

#include "LoadTexture.h"

void ProcessTest::Create() {
	Scene::Create();

	// post processing volume
	{
		const unsigned volume = entities->Create();

		//DotVFX* const dot = entities->AddComponent<DotVFX>(volume);
		//dot->SetActive(true);
		//dot->size = 100.f;

		//PopVFX* const pop = entities->AddComponent<PopVFX>(volume);
		//pop->SetActive(true);

		//PaintVFX* const paint = entities->AddComponent<PaintVFX>(volume);
		//paint->SetActive(true);
		//paint->unit = 1.f;
		//paint->radius = 12.f;

		Halftone* const halftone = entities->AddComponent<Halftone>(volume);
		halftone->SetActive(true);
		halftone->size = 200.f;

		//entities->AddComponent<Pixelated>(volume)->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		Transform* const transform = entities->GetComponent<Transform>(entity);
		transform->SetScale(vec3f(5.4f, 8.1f, 1.f) * 12.f / 8.1f);
		//transform->SetScale(vec3f(18.f, 12.f, 1.f));
		
		SpriteRender* const render = entities->AddComponent<SpriteRender>(entity);
		render->SetActive(true);
		//render->SetSprite(Load::Texture2D("Files/Textures/japan.jpg"));
		//render->SetSprite(Load::Texture2D("Files/Textures/SF_S1E1.png"));
		//render->SetSprite(Load::Texture2D("Files/Textures/test3.jpg"));
		render->SetSprite(Load::Texture2D("Files/Textures/chris.png"));
	}
}
