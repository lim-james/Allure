#include "PlayerController.h"

#include "InputEvents.h"
#include "ProjectDefines.h"
#include "Layers.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

void PlayerController::SetView(Camera * camera) {
	view = camera;
	viewTransform = entities->GetComponent<Transform>(camera->entity);
}

void PlayerController::SetHealthRender(SpriteRender * const r) {
	healthRender = r;
	healthCone = static_cast<Material::Cone*>(r->GetMaterial());
	healthAnimation = entities->GetComponent<Animator>(r->entity);
}

void PlayerController::SetShieldRender(SpriteRender * const r) {
	shieldRender = r;
	shieldCone = static_cast<Material::Cone*>(shieldRender->GetMaterial());
	shieldTransform = entities->GetComponent<Transform>(r->entity);
}

void PlayerController::SetMaxHealth(float const & value) {
	maxHealth = value;
	healthInv = 180.f / value;
}

void PlayerController::OnEnterPlayer(unsigned target) {
	if (entities->GetLayer(target) == BULLET) {
		Transform* const bTransform = entities->GetComponent<Transform>(target);
		const vec3f position = bTransform->GetWorldTranslation();
		const vec3f direction = Math::Normalized(position - transform->GetWorldTranslation());
		EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));

		vec4f& tint = healthRender->tint;

		healthAnimation->Queue(AnimationBase(false, 0.15f, 0.f, Handler<void, void>([&tint]() {
			tint = COLOR_BLACK;
		})), &tint, COLOR_RED);

		--health;
		UpdateHealth();

		BeatCollisionHandler* const handler = entities->GetComponent<BeatCollisionHandler>(target);
		handler->OnHit(entity);

		beats.erase(beats.begin());
		//beats. entities->GetComponent<BeatCollisionHandler>(target)] = 0.f;
	}
}

void PlayerController::OnEnterRange(unsigned target) {
	if (entities->GetLayer(target) == BULLET) {
		beats.push_back(entities->GetComponent<BeatCollisionHandler>(target));
	}
}

void PlayerController::Awake() {
	EventsManager::Get()->Subscribe("KEY_INPUT", &PlayerController::KeyHandler, this);
	EventsManager::Get()->Subscribe("MOUSE_BUTTON_INPUT", &PlayerController::MouseButtonHandler, this);
	EventsManager::Get()->Subscribe("CURSOR_POSITION_INPUT", &PlayerController::CursorPositionHandler, this);
}

void PlayerController::Start() {
	health = maxHealth;
	UpdateHealth();

	isShielding = false;
}

void PlayerController::Update() {
	const vec3f position = viewTransform->GetLocalTranslation();
	const vec2f destination = Math::Lerp(position.xy, vec2f(0.f), time->dt * 10.f);
	viewTransform->SetLocalTranslation(vec3f(destination, position.z));
}

void PlayerController::KeyHandler(Events::Event * event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);

	void(PlayerController::*press)() = nullptr;
	void(PlayerController::*hold)() = nullptr;
	void(PlayerController::*release)() = nullptr;

	switch (input->key) {
	case GLFW_KEY_Z:
	case GLFW_KEY_X:
		press = &PlayerController::BeatPress;
		hold = &PlayerController::BeatHold;
		release = &PlayerController::BeatRelease;
		break;
	case GLFW_KEY_SPACE:
		press = &PlayerController::WavePress;
		hold = &PlayerController::WaveHold;
		release = &PlayerController::WaveRelease;
		break;
	default:
		return;
	}

	switch (input->action) {
	case GLFW_PRESS:
		(this->*press)();
		break;
	case GLFW_REPEAT:
		(this->*hold)();
		break;
	case GLFW_RELEASE:
		(this->*release)();
		break;
	default:
		break;
	}
}

void PlayerController::MouseButtonHandler(Events::Event * event) {
	Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
}

void PlayerController::CursorPositionHandler(Events::Event * event) {
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	const vec2f position = view->ScreenToWorldPosition(input->position);
	shieldTransform->SetLocalRotation(vec3f(0.f, 0.f, atan2f(position.y, position.x) * Math::toDeg));
}

void PlayerController::BeatPress() {
	SetShieldState(true);
	
	const vec3f shieldPosition = shieldTransform->GetWorldTranslation();
	const float angle = shieldTransform->GetLocalRotation().z;

	for (unsigned i = 0; i < beats.size(); ++i) {
		BeatCollisionHandler* const beat = beats[i];

		const vec3f direction = beat->transform->GetWorldTranslation() - shieldPosition;
		const float theta = fabs(atan2f(direction.y, direction.x) * Math::toDeg - angle);

		if (theta <= shieldCone->GetRange()) {
			beat->OnHit(entity);
			SetShieldState(false);
			beats.erase(beats.begin() + i);
			return;
		}
	}
}

void PlayerController::BeatHold() {}

void PlayerController::BeatRelease() {
	SetShieldState(false);
}

void PlayerController::WavePress() {
	indicatorRender->tint = COLOR_BLACK;
}

void PlayerController::WaveHold() {

}

void PlayerController::WaveRelease() {
	indicatorRender->tint = COLOR_LIGHT_GREY;
}

void PlayerController::UpdateHealth() {
	healthCone->SetRange(health * healthInv);
}

void PlayerController::SetShieldState(bool const & state) {
	isShielding = state;
	shieldRender->tint = isShielding ? COLOR_BLACK : COLOR_LIGHT_GREY;
}

