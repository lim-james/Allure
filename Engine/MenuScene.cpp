#include "MenuScene.h"

// Systems
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"
#include "ParticleSystem.h"
// Utils
#include "LoadFNT.h"
#include "LoadTGA.h"
#include "InputEvents.h"
// Destination
#include "HexScene.h"

#include <Events/EventsManager.h>

void MenuScene::Awake() {
	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &MenuScene::CursorPositionHandler, this);

	auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	systems->Subscribe<RenderSystem>(0);
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);
	systems->Subscribe<ParticleSystem>(3);

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
		titleText = entities->AddComponent<Text>(label);
		titleText->SetActive(true);
		titleText->SetFont(font);
		titleText->text = "Allure 2D"; 
	}

	buttonSize.Set(5.f, 1.5f);

	CreateButton("PvP", vec2f(0.f, -2.f), buttonSize, vec3f(1.f))->BindHandler(MOUSE_CLICK, [this](unsigned target) {
		chosenMode = PVP;
		Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("HEX"));
	});

	CreateButton("PvE", vec2f(0.f, -4.f), buttonSize, vec3f(1.f))->BindHandler(MOUSE_CLICK, [this](unsigned target) {
		chosenMode = PVE;
		Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("HEX"));
	});

	CreateButton("AIvAI", vec2f(0.f, -6.f), buttonSize, vec3f(1.f))->BindHandler(MOUSE_CLICK, [this](unsigned target) {
		chosenMode = AIvAI;
		Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::String("HEX"));
	});
}

void MenuScene::Update(const float & dt) {
	Scene::Update(dt);

	const int FPS = static_cast<int>(1.f / dt);
	debugText->text = "FPS: " + std::to_string(FPS) + "\nCOUNT: " + std::to_string(entities->PoolCount());
}

void MenuScene::SetTitle(const std::string & title) {
	titleText->text = title;
}

Button* MenuScene::CreateButton(const std::string & title, const vec2f & position, const vec2f & size, const vec3f& color) {
	const unsigned entity = entities->Create();

	auto transform = entities->GetComponent<Transform>(entity);
	transform->translation = vec3f(position, 0.f);
	transform->scale = vec3f(size, 1.f);

	auto render = entities->AddComponent<Render>(entity);
	render->SetActive(true);
	render->tint = vec4f(color, 0.5f);

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

	auto animation = entities->AddComponent<Animation>(entity);
	animation->SetActive(true);

	return button;
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

void MenuScene::CursorPositionHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);

	auto position = camera->ScreenToWorldSpace(input->position);
	position.y = -position.y;
	//cursor->translation = position;
}

void MenuScene::PrepareForSegue(Scene * destination) {
	HexScene* dest = dynamic_cast<HexScene*>(destination);
	if (dest) {
		dest->SetMode(chosenMode);
		dest->InitializeGame();
	}
}

