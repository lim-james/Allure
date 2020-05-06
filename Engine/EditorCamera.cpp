#include "EditorCamera.h"

// components
#include "VoxelRender.h"
#include "BoxCollider.h"
// events
#include "InputEvents.h"
#include "RayCast.h"
// utils
#include "Layers.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void EditorCamera::Awake() {
	speed = 10.f;
	controlling = false;
	direction = vec3f(0.f);

	camera = GetComponent<Camera>();
	
	EventsManager* const em = EventsManager::Get();
	em->Subscribe("KEY_INPUT", &EditorCamera::KeyHandler, this);
	em->Subscribe("MOUSE_BUTTON_INPUT", &EditorCamera::MouseButtonHandler, this);
	em->Subscribe("CURSOR_POSITION_INPUT", &EditorCamera::CursorPositionHandler, this);
}

void EditorCamera::Update() {
	const vec3f d = direction * speed * time->dt;
	transform->translation += transform->GetLocalFront() * d.z;
	transform->translation += transform->GetLocalRight() * d.x;
	transform->translation += transform->GetLocalUp() * d.y;
}

void EditorCamera::KeyHandler(Events::Event * event) {
	if (!controlling) return;

	const auto input = static_cast<Events::KeyInput*>(event);

	if (input->action == GLFW_PRESS) {
		if (input->key == GLFW_KEY_W)
			direction.z += 1.0f;
		if (input->key == GLFW_KEY_S)
			direction.z -= 1.0f;

		if (input->key == GLFW_KEY_A)
			direction.x -= 1.0f;
		if (input->key == GLFW_KEY_D)
			direction.x += 1.0f;

		if (input->key == GLFW_KEY_Q)
			direction.y -= 1.0f;
		if (input->key == GLFW_KEY_E)
			direction.y += 1.0f;
	}

	if (input->action == GLFW_RELEASE) {
		if (input->key == GLFW_KEY_W)
			direction.z -= 1.0f;
		if (input->key == GLFW_KEY_S)
			direction.z += 1.0f;

		if (input->key == GLFW_KEY_A)
			direction.x += 1.0f;
		if (input->key == GLFW_KEY_D)
			direction.x -= 1.0f;

		if (input->key == GLFW_KEY_Q)
			direction.y += 1.0f;
		if (input->key == GLFW_KEY_E)
			direction.y -= 1.0f;
	}
}

void EditorCamera::MouseButtonHandler(Events::Event * event) {
	const auto input = static_cast<Events::MouseButtonInput*>(event);
	
	if (input->button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (input->action == GLFW_PRESS) {
			EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED));
			controlling = true;
		} else if (input->action == GLFW_RELEASE) {
			EventsManager::Get()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL));
			controlling = false;
			direction = vec3f(0.0f);
		}
	} else if (input->button == GLFW_MOUSE_BUTTON_LEFT) {
		if (input->action == GLFW_PRESS) {
			CollisionData result;

			auto rayCast = new Events::RayCast(
				transform->GetWorldTranslation(),
				ScreenToRay(cursorPosition),
				BitField(DEFAULT),
				&result
			);
			EventsManager::Get()->Trigger("RAY_CAST", rayCast);

			if (result.target) {
				Transform* const targetTransform = entities->GetComponent<Transform>(result.target->entity);
				const vec3f targetPosition = targetTransform->GetWorldTranslation();

				unsigned box = entities->Create();

				Transform* const boxTransform = entities->AddComponent<Transform>(box);
				boxTransform->translation = targetPosition + result.normal;
				boxTransform->SetDynamic(false);

				VoxelRender* const render = entities->AddComponent<VoxelRender>(box);
				render->SetActive(true);
				render->tint = vec4f(Math::RandValue(), Math::RandValue(), Math::RandValue(), 1.f);
				render->SetDynamic(false);

				BoxCollider* const collider = entities->AddComponent<BoxCollider>(box);
				collider->SetActive(true);
			}
		}
	}
}

void EditorCamera::CursorPositionHandler(Events::Event * event) {
	const auto input = static_cast<Events::CursorPositionInput*>(event);
	cursorPosition = input->position;

	if (!controlling) return;
	transform->rotation.y -= input->delta.x;
	transform->rotation.x -= input->delta.y;
	transform->UpdateAxes();
}

vec3f EditorCamera::ScreenToRay(vec2f const & screenPosition) {
	mat4f invProjection = camera->GetProjectionMatrix();
	Math::Inverse(invProjection);

	mat4f invView = transform->GetLocalLookAt();
	Math::Inverse(invView);

	// normal device coordinates
	vec4f rayClip = vec4f((2.0f * screenPosition) / camera->GetViewport().size, -1.f, 1.f);
	rayClip.x = rayClip.x - 1.f;
	rayClip.y = 1.f - rayClip.y;

	vec4f rayEye = invProjection * rayClip;
	rayEye = vec4f(rayEye.xy, -1.f, 0.f);

	vec3f rayWorld = invView * rayEye;
	Math::Normalize(rayWorld);

	return rayWorld;
}
