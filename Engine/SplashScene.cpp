#include "SplashScene.h"

// systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"

#include "LoadFNT.h"
#include "InputEvents.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void SplashScene::Awake()
{
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);

	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &SplashScene::KeyDownHander, this);

	camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetActive(true);
	camera->clearColor.Set(0.f);
	entities->GetComponent<Camera>(mainCamera)->SetSize(10.f);

	bt = 0.f;
	doOnce01 = false;

	const unsigned label = entities->Create();
	//entities->GetComponent<Transform>(label)->translation.Set(-17.f, -7.5f, 0.f);
	const auto text = entities->AddComponent<Text>(label);
	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	text->SetActive(true);
	//text->paragraphAlignment = PARAGRAPH_LEFT;
	text->paragraphAlignment = PARAGRAPH_CENTER;
	text->verticalAlignment = ALIGN_MIDDLE;
	text->scale = 1.5f;
	text->text = "AGDev Assignment Showcase";
	text->color.a = 0.0f;
	textID = text->entity;
}

void SplashScene::Update(const float & dt)
{
	Scene::Update(dt);

	if (entities->GetComponent<Text>(textID)->color.a <= 1.0f && !doOnce01) {
		entities->GetComponent<Text>(textID)->color.a += 0.5 * dt;
		if (entities->GetComponent<Text>(textID)->color.a >= 1.0f)
			doOnce01 = true;
	}
	else if (bt < 2.f) {
		bt += dt;
	}
	else if (entities->GetComponent<Text>(textID)->color.a >= 0.0f && doOnce01) {
		entities->GetComponent<Text>(textID)->color.a -= 0.5 * dt;
	}
	else {
		Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("PE"));
	}
	
}

void SplashScene::KeyDownHander(Events::Event* event)
{
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	if (input->key == 'W' && input->action == GLFW_RELEASE)
		Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("NN"));
}
