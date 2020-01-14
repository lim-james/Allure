#include "DebugScene.h"

#include "Transform.h"

#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "AnimationSystem.h"

#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void DebugScene::Awake() {
	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &DebugScene::KeyHandler, this);

	systems->Subscribe<RenderSystem>(0);
	systems->Subscribe<ButtonSystem>(1);
	systems->Subscribe<AnimationSystem>(2);

	const unsigned cam = entities->Create();
	entities->GetComponent<Transform>(cam)->translation.z = 1.f;
	camera = entities->AddComponent<Camera>(cam);
	camera->SetSize(5);
	camera->SetActive(true);
	camera->clearColor.Set(0.f);

	square = entities->Create();
	entities->AddComponent<Render>(square)->SetActive(true);
	entities->AddComponent<Animation>(square)->SetActive(true);
	{
		auto button = entities->AddComponent<Button>(square);
		button->SetActive(true);
		button->BindHandler(MOUSE_OVER, &DebugScene::OnMouseOver, this);
		button->BindHandler(MOUSE_OUT, &DebugScene::OnMouseOut, this);
	}
}

void DebugScene::KeyHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_SPACE) {
		if (input->action == GLFW_PRESS) {
			entities->GetComponent<Animation>(square)->Animate(
				AnimationBase(false, 0.2), 
				entities->GetComponent<Transform>(square)->scale,
				vec3f(3.f)
			);
		} else if (input->action == GLFW_RELEASE) {
			entities->GetComponent<Animation>(square)->Animate(
				AnimationBase(false, 0.2), 
				entities->GetComponent<Transform>(square)->scale,
				vec3f(1.f)
			);
		}
	}
	
}

void DebugScene::OnMouseOver(unsigned target) {
	entities->GetComponent<Animation>(target)->Animate(
		AnimationBase(false, 0.2), 
		entities->GetComponent<Transform>(target)->scale,
		vec3f(2.75f)
	);
	Console::Warn << "Mouse over " << target << '\n';
}

void DebugScene::OnMouseOut(unsigned target) {
	entities->GetComponent<Animation>(target)->Animate(
		AnimationBase(false, 0.2), 
		entities->GetComponent<Transform>(target)->scale,
		vec3f(3.f)
	);
	Console::Warn << "Mouse out " << target << '\n';
}

