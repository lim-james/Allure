#include "EditorManager.h"

#include "InputEvents.h"
#include "ProjectDefines.h"

#include <Math/Math.hpp>
#include <Math/Random.hpp>
#include <Helpers/FileHelpers.h>
#include <Events/EventsManager.h>

void EditorManager::Awake() {
	EventsManager::Get()->Subscribe("DROP_INPUT", &EditorManager::DropHandler, this);
}

void EditorManager::Start() {
	alertBt = 0.f;
}

void EditorManager::Update() {
	if (alertBt > 0.f) {
		alertBt -= time->dt;
		background->tint = Math::Lerp(vec4f(1.f, 1.f, 1.f, 0.15f), COLOR_RED, alertBt);
		if (alertBt <= 0.f) {
			promptLabel->text = "DRAG IN YOUR WAV FILE.";
		}
	}
}

void EditorManager::DropHandler(Events::Event * event) {
	Events::DropInput* const input = static_cast<Events::DropInput*>(event);

	const std::string path = input->paths[0];
	if (Helpers::GetFileExt(path) != "wav") {
		EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(vec2f(
			2.f * Math::RandValue() * static_cast<float>(Math::RandSign()),
			2.f * Math::RandValue() * static_cast<float>(Math::RandSign())
		)));

		promptLabel->text = "INVALID FILE TYPE.";
		background->tint = COLOR_RED;
		alertBt = 1.f;
		return;
	}

	animator->Queue(AnimationBase(false, 0.25f), &promptLabel->color.a, 0.f);
	animator->Queue(AnimationBase(false, 0.25f), &avLayout->GetConstraint(BOTTOM_ANCHOR)->constant, 0.f);
	Transform* const pTransform = entities->GetComponent<Transform>(promptLabel->entity);
	tAnimator->Queue(AnimationBase(false, 0.5f), pTransform, ANIMATE_TRANSLATION, vec3f(0.f, -0.25f, 0.f));

	controller->SetTrack(path);
}
