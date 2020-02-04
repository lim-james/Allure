#include "TitleScene.h"

// components
#include "Render.h"
// Systems
#include "SpriteAnimationSystem.h"
// Utils
#include "LoadFNT.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>

void TitleScene::Awake() {
	Scene::Awake();

	auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	systems->Subscribe<SpriteAnimationSystem>(1);

	// debug text
	{
		const unsigned label = entities->Create();
		entities->GetComponent<Transform>(label)->translation.Set(-15.f, 8.f, 0.f);
		debugText = entities->AddComponent<Text>(label);
		debugText->SetActive(true);
		debugText->SetFont(font);
		debugText->scale = 0.5f;
		debugText->paragraphAlignment = PARAGRAPH_LEFT;
		debugText->verticalAlignment = ALIGN_TOP;
	}

	// title
	{
		const unsigned label = entities->Create();
		titleText = entities->AddComponent<Text>(label);
		titleText->SetActive(true);
		titleText->SetFont(font);
		titleText->text = "Allure 2D"; 
	}

	{
		const unsigned icon = entities->Create();
		entities->GetComponent<Transform>(icon)->translation.Set(0.f, -2.f, 0.f);

		auto render = entities->AddComponent<Render>(icon);
		render->SetActive(true);
		render->SetTexture("Files/Fonts/Microsoft.tga");

		auto sprite = entities->AddComponent<SpriteAnimation>(icon);
		sprite->SetActive(true);

		Keyframe kf;
		kf.SetTilemapSize(16, 16);
		kf.duration = 0.1f;

		SpriteAnimationData anim;
		for (int x = 0; x < 16; ++x) {
			for (int y = 0; y < 16; ++y) {
				kf.SetCellRect(x, y, 1, 1);
				anim.frames.push_back(kf);
			}
		}

		sprite->animations["MAIN"] = anim;
		sprite->currentAnimation = "MAIN";
	}
}

void TitleScene::Update(const float & dt) {
	Scene::Update(dt);

	const int FPS = static_cast<int>(1.f / dt);
	debugText->text = "FPS: " + std::to_string(FPS) + "\nCOUNT: " + std::to_string(entities->PoolCount());
}
