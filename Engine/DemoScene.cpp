#include "DemoScene.h"

#include "LoadFNT.h"
#include "Transform.h"
#include "Text.h"

#include <GLFW/glfw3.h>

void DemoScene::Create() {
	Scene::Create();

	// fonts
	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	// title
	{
		const unsigned label = entities->Create();

		auto transform = entities->GetComponent<Transform>(label);
		transform->translation.y = -0.25f;

		Text* titleText = entities->AddComponent<Text>(label);
		titleText->SetActive(true);
		titleText->SetFont(courierNew);
		titleText->text = "Demo";
		titleText->color.Set(1.f);

		auto script = entities->AddComponent<Script>(label);
		script->SetActive(true);
	}
}

