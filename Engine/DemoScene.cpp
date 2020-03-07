#include "DemoScene.h"

#include "LoadFNT.h"
#include "Transform.h"
#include "Text.h"

#include "TitleScene.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void DemoScene::Create() {
	Scene::Create();

	name = "Demo" + std::to_string(++count);

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
	}

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &DemoScene::KeyHandler, this);
}

void DemoScene::KeyHandler(Events::Event * event) {
	Events::EventsManager::GetInstance()->Trigger("DISMISS_SCENE");
	//Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	//if (input->action == GLFW_PRESS)
	//	Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::PresentScene(new TitleScene));
}

