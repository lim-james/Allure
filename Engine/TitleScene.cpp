#include "TitleScene.h"

// components
#include "Render.h"
// Systems
#include "ButtonSystem.h"
#include "ParticleSystem.h"
#include "AnimationSystem.h"
// Utils
#include "LoadTGA.h"
#include "LoadFNT.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void TitleScene::Awake() {
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<ParticleSystem>(2);
	systems->Subscribe<AnimationSystem>(3);

	Scene::Awake();

	camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);

	// fonts
	auto microsoft = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");
	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	// title
	{
		const unsigned label = entities->Create();

		auto transform = entities->GetComponent<Transform>(label);
		transform->translation.y = -0.2f;

		titleText = entities->AddComponent<Text>(label);
		titleText->SetActive(true);
		titleText->SetFont(microsoft);
		titleText->text = "Allure 2D"; 
		titleText->color.Set(1.f, 1.f, 1.f, 0.0f);

		auto animation = entities->AddComponent<Animation>(label);
		animation->SetActive(true);

		animation->Animate(
			AnimationBase(false, 0.5f, 5.f),
			titleText->color.a,
			1.f
		);

		animation->Animate(
			AnimationBase(false, 0.5f, 5.f),
			transform->translation.y,
			0.f
		);
	}

	{
		const unsigned entity = entities->Create();

		mouse = entities->GetComponent<Transform>(entity);

		const auto emitter = entities->AddComponent<ParticleEmitter>(entity);
		emitter->SetActive(true);
		emitter->texture = Load::TGA("Files/Textures/circle.tga");

		//emitter->burstAmount = 50;
		//emitter->spawnInterval = 0.2f;
		//emitter->duration = 0.1f;
		//emitter->loop = true;

		emitter->lifetime = 5.f;
		emitter->lifetimeRange = 0.5f;

		emitter->positionRange.Set(1.f, 1.f, 0.f);

		emitter->angleRange = 180.f;

		emitter->speed = 5.f;
		//emitter->speedRange = 5.f;

		emitter->accelRad = -5.f;
		//emitter->accelRadRange = 5.f;

		//emitter->gravity.Set(0.f, -4.f, 0.f);

		emitter->startSize.Set(0.1f);
		emitter->endSize.Set(0.f);

		emitter->startColor.Set(0.5f, 0.0f, 0.5f, 1.f);
		emitter->startColorRange.Set(0.5f, 0.0f, 0.f, 0.f);
		//emitter->startColor.Set(0.5f, 0.5f, 0.5f, 1.f);
		//emitter->startColorRange.Set(0.5f, 0.5f, 0.5f, 0.f);

		emitter->endColor.Set(1.f, 0.5f, 0.0f, 1.f);
		emitter->endColorRange.Set(0.f, 0.5f, 0.0f, 0.f);
		//emitter->endColor.Set(0.5f, 0.5f, 0.5f, 0.f);
		//emitter->endColorRange.Set(0.5f, 0.5f, 0.5f, 0.f);
	}

	{
		//const unsigned entity = entities->Create();

		//auto transform = entities->GetComponent<Transform>(entity);
		//transform->translation.y = -7.f;
		//transform->scale.Set(7.f, 2.f, 0.f);

		//auto render = entities->AddComponent<Render>(entity);
		//render->SetActive(true);
		//render->tint.Set(1.f);
	}

	textFieldManager = new UITextFieldManager(entities);

	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &TitleScene::CursorPositionHandler, this);
}

void TitleScene::Update(const float & dt) {
	Scene::Update(dt);

	textFieldManager->Update(dt);
}

void TitleScene::Destroy() {
	delete textFieldManager;
}

void TitleScene::CursorPositionHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);

	auto position = camera->ScreenToWorldSpace(input->position);
	position.y = -position.y;
	mouse->translation = position;
}

