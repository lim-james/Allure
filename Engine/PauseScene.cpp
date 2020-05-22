#include "PauseScene.h"

#include "Camera.h"
#include "Text.h"
#include "LayoutSystem.h"
// Scripts
#include "PauseManager.h"
#include "TextBlink.h"
// Utils
#include "LoadFNT.h"

void PauseScene::Awake() {
	Scene::Awake();

	systems->Subscribe<LayoutSystem>(1);
}

void PauseScene::Create() {
	Scene::Create();

	Font* const vcrMono = Load::FNT("Files/Fonts/vcr_ocd_mono.fnt", "Files/Fonts/vcr_ocd_mono.png");

	Camera* const camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);
	camera->projection = ORTHOGRAPHIC;

	// pause manager
	{
		const unsigned entity = entities->Create();

		PauseManager* const manager = entities->AddComponent<PauseManager>(entity);
		manager->SetActive(true);
	}

	// Pause title
	{
		const unsigned entity = entities->Create();

		Layout* const layout = entities->AddComponent<Layout>(entity);
		layout->SetActive(true);
		layout->AddConstraint(Constraint(CENTER_X_ANCHOR, nullptr, CENTER_X_ANCHOR, 1.f, 0.f, camera));
		layout->AddConstraint(Constraint(CENTER_Y_ANCHOR, nullptr, CENTER_Y_ANCHOR, 1.f, 0.f, camera));

		Text* const text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(vcrMono);
		text->text = "PAUSED";

		TextBlink* const blink = entities->AddComponent<TextBlink>(entity);
		blink->SetActive(true);
		blink->duration = 0.5f;
	}
}
