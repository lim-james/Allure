#include "TitleScene.h"

// Systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"
#include "ParticleSystem.h"
// Utils
#include "LoadFNT.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>

void TitleScene::Awake() {
	Scene::Awake();

	auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);
	systems->Subscribe<ParticleSystem>(3);

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

	buttonSize.Set(5.f, 1.5f);
}

void TitleScene::Update(const float & dt) {
	Scene::Update(dt);

	const int FPS = static_cast<int>(1.f / dt);
	debugText->text = "FPS: " + std::to_string(FPS) + "\nCOUNT: " + std::to_string(entities->PoolCount());
}
