#include "MenuScene.h"

#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"
#include "ParticleSystem.h"

#include "LoadFNT.h"
#include "LoadTGA.h"
#include "InputEvents.h"

#include <Events/EventsManager.h>

void MenuScene::Awake() {
	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &MenuScene::CursorPositionHandler, this);

	auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	systems->Subscribe<RenderSystem>(0);
	//systems->Subscribe<ButtonSystem>(1);
	//systems->Subscribe<AnimationSystem>(2);
	systems->Subscribe<ParticleSystem>(1);

	const unsigned cam = entities->Create();
	entities->GetComponent<Transform>(cam)->translation.z = 1.f;
	camera = entities->AddComponent<Camera>(cam);
	camera->SetActive(true);
	camera->SetSize(10);
	camera->clearColor.Set(0.f);

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
		const auto text = entities->AddComponent<Text>(label);
		text->SetActive(true);
		text->SetFont(font);
		text->text = "Allure 2D"; 
	}

	// cursor
	{
		const unsigned entity = entities->Create();

		cursor = entities->GetComponent<Transform>(entity);

		const auto emitter = entities->AddComponent<ParticleEmitter>(entity);
		emitter->SetActive(true);
		//emitter->texture = Load::TGA("Files/Textures/circle.tga");

		emitter->burstAmount = 50;
		//emitter->spawnInterval = 0.2f;
		//emitter->duration = 0.1f;
		//emitter->loop = true;

		emitter->lifetime = 15.f;
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

	buttonSize.Set(2.f, 1.f);

	//CreateButton("Play", vec2f(0.f, -2.f), buttonSize, vec4f(1.f));
}

void MenuScene::Update(const float & dt) {
	Scene::Update(dt);

	const int FPS = static_cast<int>(1.f / dt);
	avg = (FPS + avg) * 0.5f;
	debugText->text = "FPS: " + std::to_string(FPS) + "\nCOUNT: " + std::to_string(entities->PoolCount());
}

unsigned MenuScene::CreateButton(const std::string & title, const vec2f & position, const vec2f & size, const vec4f& color) {
	const unsigned entity = entities->Create();

	auto transform = entities->GetComponent<Transform>(entity);
	transform->translation = vec3f(position, 0.f);
	transform->scale = vec3f(size, 1.f);

	auto render = entities->AddComponent<Render>(entity);
	render->SetActive(true);
	render->tint = color;

	auto text = entities->AddComponent<Text>(entity);
	text->SetActive(true);
	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	text->text = title;
	text->color.Set(0.f, 0.f, 0.f, 1.f);
	text->scale = 0.5f;

	auto button = entities->AddComponent<Button>(entity);
	button->SetActive(true);
	button->BindHandler(MOUSE_OVER, &MenuScene::OnMouseOverHandler, this);
	button->BindHandler(MOUSE_OUT, &MenuScene::OnMouseOutHandler, this);
	button->BindHandler(MOUSE_DOWN, &MenuScene::OnMouseDownHandler, this);
	button->BindHandler(MOUSE_UP, &MenuScene::OnMouseUpHandler, this);
	button->BindHandler(MOUSE_CLICK, &MenuScene::OnClick, this);

	auto animation = entities->AddComponent<Animation>(entity);
	animation->SetActive(true);

	return entity;
}

void MenuScene::OnMouseOverHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Render>(entity)->tint.a,
		1.f
	);
}

void MenuScene::OnMouseOutHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Render>(entity)->tint.a,
		0.5f
	);
}

void MenuScene::OnMouseDownHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(buttonSize * 0.9f, 1.f)
	);
}

void MenuScene::OnMouseUpHandler(unsigned entity) {
	entities->GetComponent<Animation>(entity)->Animate(
		AnimationBase(false, 0.2f),
		entities->GetComponent<Transform>(entity)->scale,
		vec3f(buttonSize, 1.f)
	);
}

void MenuScene::OnClick(unsigned entity) {
	std::cout << "Entity count : " << entities->PoolCount() << '\n';
}

void MenuScene::CursorPositionHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);

	auto position = camera->ScreenToWorldSpace(input->position);
	position.y = -position.y;
	cursor->translation = position;
}

