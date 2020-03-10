#include "TitleScene.h"

// components
#include "Render.h"
#include "ButtonAnimation.h"
#include "FollowCursor.h"
#include "TitleTransition.h"
// Systems
#include "ButtonSystem.h"
#include "ParticleSystem.h"
#include "AnimationSystem.h"
// Utils
#include "LoadFNT.h"

void TitleScene::Awake() {
	Scene::Awake();

	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<ParticleSystem>(2);
	systems->Subscribe<AnimationSystem>(3);
}

void TitleScene::Create() {
	Scene::Create();

	Camera* camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);

	// fonts
	auto microsoft = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");
	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	// manager 
	const unsigned manager = entities->Create();
	auto transition = entities->AddComponent<TitleTransition>(manager);
	transition->SetActive(true);

	// title
	{
		const unsigned label = entities->Create();

		auto transform = entities->GetComponent<Transform>(label);
		transform->translation.y = -0.25f;
		transition->titleTransform = transform;

		auto text = entities->AddComponent<Text>(label);
		text->SetActive(true);
		text->SetFont(microsoft);
		text->text = "Allure 2D"; 
		text->color.Set(1.f);

		auto animation = entities->AddComponent<Animation>(label);
		animation->SetActive(true);

		auto script = entities->AddComponent<Script>(label);
		script->SetActive(true);
	}

	// open button
	{
		const unsigned entity = entities->Create();

		auto transform = entities->GetComponent<Transform>(entity);
		transform->translation.y = -7.25f;
		transform->scale.Set(7.f, 1.5f, 0.f);
		transition->buttonTransform = transform;

		auto render = entities->AddComponent<Render>(entity);
		render->SetActive(true);
		render->tint.Set(1.f, 1.f, 1.f, 0.5f);

		auto text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(microsoft);
		text->text = "Open canvas"; 
		text->scale = 0.5f;
		text->color.Set(0.f, 0.f, 0.f, 1.f);

		auto animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);

		auto button = entities->AddComponent<Button>(entity);
		button->SetActive(true);
		button->handlers[MOUSE_CLICK].Bind(&TitleTransition::Transition, transition);

		auto buttonAnim = entities->AddComponent<ButtonAnimation>(entity);
		buttonAnim->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		const auto emitter = entities->AddComponent<ParticleEmitter>(entity);
		emitter->SetActive(true);
		transition->emitter = emitter;

		emitter->lifetime = 5.f;
		emitter->lifetimeRange = 0.5f;

		emitter->positionRange.Set(1.f, 1.f, 0.f);

		emitter->angleRange = 180.f;

		emitter->speed = 5.f;

		emitter->accelRad = -5.f;

		emitter->startSize.Set(0.1f);
		emitter->endSize.Set(0.f);

		emitter->startColor.Set(0.5f, 0.0f, 0.5f, 1.f);
		emitter->startColorRange.Set(0.5f, 0.0f, 0.f, 0.f);

		emitter->endColor.Set(1.f, 0.5f, 0.0f, 1.f);
		emitter->endColorRange.Set(0.f, 0.5f, 0.0f, 0.f);

		auto follow = entities->AddComponent<FollowCursor>(entity);
		follow->SetActive(true);
		follow->camera = entities->GetComponent<Camera>(mainCamera);
	}
}
