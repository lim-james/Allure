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
#include "EditorScene.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void TitleScene::Awake() {
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<ParticleSystem>(2);
	systems->Subscribe<AnimationSystem>(3);

	Scene::Awake();

	destination = "";
	transitionDelay = 0.f;

	camera = entities->GetComponent<Camera>(mainCamera);
	camera->SetSize(10.f);

	// fonts
	auto microsoft = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");
	auto courierNew = Load::FNT("Files/Fonts/CourierNew.fnt", "Files/Fonts/CourierNew.tga");

	// title
	{
		const unsigned label = entities->Create();

		auto transform = entities->GetComponent<Transform>(label);
		transform->translation.y = -0.25f;

		titleText = entities->AddComponent<Text>(label);
		titleText->SetActive(true);
		titleText->SetFont(microsoft);
		titleText->text = "Allure 2D"; 
		titleText->color.Set(1.f, 1.f, 1.f, 0.0f);

		auto animation = entities->AddComponent<Animation>(label);
		animation->SetActive(true);
	}

	// open button
	{
		const unsigned entity = entities->Create();

		auto transform = entities->GetComponent<Transform>(entity);
		transform->translation.y = -7.25f;
		transform->scale.Set(7.f, 1.5f, 0.f);

		buttonRender = entities->AddComponent<Render>(entity);
		buttonRender->SetActive(true);
		buttonRender->tint.Set(1.f, 1.f, 1.f, 0.f);

		auto text = entities->AddComponent<Text>(entity);
		text->SetActive(true);
		text->SetFont(microsoft);
		text->text = "Open canvas"; 
		text->scale = 0.5f;
		text->color.Set(0.f, 0.f, 0.f, 1.f);

		auto button = entities->AddComponent<Button>(entity);
		button->BindHandler(MOUSE_OVER, &TitleScene::OnMouseOver, this);
		button->BindHandler(MOUSE_OUT, &TitleScene::OnMouseOut, this);
		button->BindHandler(MOUSE_DOWN, &TitleScene::OnMouseDown, this);
		button->BindHandler(MOUSE_UP, &TitleScene::OnMouseUp, this);
		button->BindHandler(MOUSE_CLICK, &TitleScene::OnClick, this);

		auto animation = entities->AddComponent<Animation>(entity);
		animation->SetActive(true);
	}

	{
		const unsigned entity = entities->Create();

		mouse = entities->GetComponent<Transform>(entity);

		const auto emitter = entities->AddComponent<ParticleEmitter>(entity);
		emitter->SetActive(true);
		//emitter->texture = Load::TGA("Files/Textures/circle.tga");

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

	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &TitleScene::CursorPositionHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("DROP_INPUT", &TitleScene::DropHandler, this);
}

void TitleScene::Start() {
	entities->GetComponent<ParticleEmitter>(mouse->entity)->SetActive(true);

	{
		auto transform = entities->GetComponent<Transform>(titleText->entity);
		auto animation = entities->GetComponent<Animation>(titleText->entity);

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
		auto button = entities->GetComponent<Button>(buttonRender->entity);
		auto transform = entities->GetComponent<Transform>(buttonRender->entity);
		auto animation = entities->GetComponent<Animation>(buttonRender->entity);

		animation->Animate(
			AnimationBase(false, 0.5f, 5.f, [button]() {
				button->SetActive(true);
			}),
			buttonRender->tint.a,
			0.5f
		);

		animation->Animate(
			AnimationBase(false, 0.5f, 5.f),
			transform->translation.y,
			-7.f
		);
	}
}

void TitleScene::Update(const float & dt) {
	Scene::Update(dt);

	if (!destination.empty()) {
		transitionDelay -= dt;
		if (transitionDelay <= 0.f) {
			Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String(destination));
			destination = "";
			transitionDelay = 0.0f;
		}
	}
}

void TitleScene::PrepareForSegue(Scene * destination) {
	EditorScene* scene = dynamic_cast<EditorScene*>(destination);
	if (scene) {
		scene->SetFile(file);
	}
}

void TitleScene::CursorPositionHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);

	auto position = camera->ScreenToWorldSpace(input->position);
	position.y = -position.y;
	mouse->translation = position;
}

void TitleScene::DropHandler(Events::Event * event) {
	auto drop = static_cast<Events::DropInput*>(event);

	file = drop->paths[0];

	FadeOut();
	destination = "EDITOR";
	transitionDelay = 5.f;
}

void TitleScene::OnMouseOver(unsigned target) {
	auto render = entities->GetComponent<Render>(target);
	auto animation = entities->GetComponent<Animation>(target);

	animation->Animate(
		AnimationBase(false, 0.2f),
		render->tint.a,
		1.f
	);
}

void TitleScene::OnMouseOut(unsigned target) {
	auto render = entities->GetComponent<Render>(target);
	auto animation = entities->GetComponent<Animation>(target);

	animation->Animate(
		AnimationBase(false, 0.2f),
		render->tint.a,
		0.5f
	);
}

void TitleScene::OnMouseDown(unsigned target) {
	auto transform = entities->GetComponent<Transform>(target);
	auto animation = entities->GetComponent<Animation>(target);

	animation->Animate(
		AnimationBase(false, 0.2f),
		transform->scale,
		vec3f(7.f, 1.5f, 0.f) * 0.9f
	);
}

void TitleScene::OnMouseUp(unsigned target) {
	auto transform = entities->GetComponent<Transform>(target);
	auto animation = entities->GetComponent<Animation>(target);

	animation->Animate(
		AnimationBase(false, 0.2f),
		transform->scale,
		vec3f(7.f, 1.5f, 0.f)
	);
}

void TitleScene::OnClick(unsigned target) {
	FadeOut();
	destination = "CANVAS";
	transitionDelay = 5.f;
}

void TitleScene::FadeOut() {
	entities->GetComponent<Button>(buttonRender->entity)->SetActive(false);
	entities->GetComponent<ParticleEmitter>(mouse->entity)->SetActive(false);

	{
		auto transform = entities->GetComponent<Transform>(titleText->entity);
		auto animation = entities->GetComponent<Animation>(titleText->entity);

		animation->Animate(
			AnimationBase(false, 0.5f),
			titleText->color.a,
			0.f
		);

		animation->Animate(
			AnimationBase(false, 0.5f),
			transform->translation.y,
			0.25f
		);
	}

	{
		auto transform = entities->GetComponent<Transform>(buttonRender->entity);
		auto animation = entities->GetComponent<Animation>(buttonRender->entity);

		animation->Animate(
			AnimationBase(false, 0.5f),
			buttonRender->tint.a,
			0.f
		);

		animation->Animate(
			AnimationBase(false, 0.5f),
			transform->translation.y,
			-7.25f
		);
	}
}

