#include "Test.h"

#include "InputEvents.h"
#include "Transform.h"
#include "AIAttribute.h"
#include "RenderSystem.h"
#include "ColliderSystem.h"
#include "ParticleSystem.h"
#include "StateMachine.h"
#include "LoadFNT.h"

#include "RoamState.h"

#include <Events/EventsManager.h>
#include <Math/Random.hpp>
#include <GLFW/glfw3.h>

void Test::Awake() {
	Scene::Awake();

	auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	systems->Subscribe<RenderSystem>(0);
	systems->Subscribe<ParticleSystem>(1);
	systems->Subscribe<ColliderSystem>(2);
	systems->Subscribe<StateMachine>(3);

	systems->Get<StateMachine>()->AttachState<States::RoamState>("ROAM");

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &Test::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &Test::CursorHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &Test::MouseButtonHandler, this);

	const unsigned cam = entities->Create();
	entities->GetComponent<Transform>(cam)->translation.z = 1.f;
	camera = entities->AddComponent<Camera>(cam);
	camera->SetSize(10);
	camera->SetActive(true);
	camera->clearColor.Set(0.5f);

	const unsigned background = entities->Create();
	entities->GetComponent<Transform>(background)->translation.z = -0.1f;
	entities->GetComponent<Transform>(background)->scale.Set(20.f);
	auto bgRender = entities->AddComponent<Render>(background);
	bgRender->SetTexture("Files/Textures/grid20.tga");
	bgRender->SetActive(true);

	const unsigned label = entities->Create();
	entities->GetComponent<Transform>(label)->translation.Set(-15.f, 8.f, 0.f);
	debugText = entities->AddComponent<Text>(label);
	debugText->SetFont(font);
	debugText->SetActive(true);
}

void Test::Update(const float & dt) {
	Scene::Update(dt);

	debugText->text = std::to_string(static_cast<int>(1.f / dt));
}

void Test::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	if (input->action == GLFW_RELEASE) {
		if (input->key == GLFW_KEY_1) {
			Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::PresentScene("ONE"));
		} else if (input->key == GLFW_KEY_2) {
			Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::PresentScene("TWO"));
		} else if (input->key == GLFW_KEY_3) {
			Events::EventsManager::GetInstance()->Trigger("PRESENT_SCENE", new Events::PresentScene("THREE"));
		} else if (input->key == GLFW_KEY_SPACE) {
			Console::Warn << "Space pressed\n";
			CreateFish(vec2f(Math::RandMinMax(-10.f, 10.f), Math::RandMinMax(-10.f, 10.f)));
		} else if (input->key == GLFW_KEY_BACKSPACE) {
			system("cls");
		}
	}
}

void Test::CursorHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);
	mousePosition = camera->ScreenToWorldSpace(input->position);
}

void Test::MouseButtonHandler(Events::Event * event) {
	auto input = static_cast<Events::MouseButtonInput*>(event);
	if (input->button == GLFW_MOUSE_BUTTON_LEFT) {
		if (input->action == GLFW_PRESS) {
			mouseDown = true;
			CreateFishFood(mousePosition);
		} else if (input->action == GLFW_RELEASE) {
			mouseDown = false;
		}
	}
}

unsigned Test::Create(const vec2f & position, const vec4f & color) {
	const unsigned id = entities->Create();
	auto transform = entities->GetComponent<Transform>(id);
	transform->translation.Set(position, 0.f);
	transform->scale.Set(0.25f);

	auto render = entities->AddComponent<Render>(id);
	render->tint = color;
	render->SetActive(true);

	auto collider = entities->AddComponent<Collider>(id);
	collider->BindHandler(COLLIDER_ENTER, &Test::CollisionEnter, this);
	collider->BindHandler(COLLIDER_EXIT, &Test::CollisionExit, this);
	collider->SetActive(true);

	auto states = entities->AddComponent<StateContainer>(id);
	states->queuedState = "ONE";
	states->SetActive(true);

	return id;
}

unsigned Test::CreateFishFood(const vec2f & position) {
	const unsigned id = entities->Create();

	auto transform = entities->GetComponent<Transform>(id);
	transform->translation.Set(
		Math::Round(position.x - 0.5f, 10, 10.f) + 0.5f, 
		Math::Round(position.y - 0.5f, 10, 10.f) + 0.5f, 
		0.f
	);
	transform->scale.Set(0.9f);

	auto ai = entities->AddComponent<AIAttribute>(id);
	ai->speed = 0.5f;
	ai->destination = transform->translation;

	auto render = entities->AddComponent<Render>(id);
	render->SetActive(true);
	render->SetTexture("Files/Textures/fishfood.tga");

	auto collider = entities->AddComponent<Collider>(id);
	collider->BindHandler(COLLIDER_ENTER, &Test::CollisionEnter, this);
	collider->BindHandler(COLLIDER_EXIT, &Test::CollisionExit, this);
	collider->SetActive(true);

	auto state = entities->AddComponent<StateContainer>(id);
	state->queuedState = "ROAM";
	state->SetActive(true); 

	auto text = entities->AddComponent<Text>(id);
	text->text = std::to_string(id);
	text->color.Set(0.f, 0.f, 0.f, 1.f);
	text->SetActive(true);
	text->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));

	return id;
}

unsigned Test::CreateFish(const vec2f & position) {
	const unsigned id = entities->Create();
	auto transform = entities->GetComponent<Transform>(id);
	transform->translation.Set(
		Math::Round(position.x - 0.5f, 10, 10.f) + 0.5f, 
		Math::Round(position.y - 0.5f, 10, 10.f) + 0.5f, 
		0.f
	);
	transform->scale.Set(0.9f);

	auto render = entities->AddComponent<Render>(id);
	render->SetActive(true);
	render->SetTexture("Files/Textures/hungry.tga");

	auto collider = entities->AddComponent<Collider>(id);
	collider->BindHandler(COLLIDER_ENTER, &Test::CollisionEnter, this);
	collider->BindHandler(COLLIDER_EXIT, &Test::CollisionExit, this);
	collider->SetActive(true);

	return id;
}

void Test::CollisionEnter(unsigned target) {
	//entities->GetComponent<Render>(target)->tint.Set(1.f, 0.f, 0.f, 1.f);
	entities->Destroy(target);
}

void Test::CollisionExit(unsigned target) {

}
