#include "PlayerLife.h"

#include "Layers.h"
#include "EnemyLife.h"
#include <Events/EventsManager.h>

void PlayerLife::OnCollisionEnter(unsigned target) {
	switch (entities->GetLayer(target)) {
	case ENEMY:
		if (inviT <= 0.f && entities->GetComponent<EnemyLife>(target)->bodyDamage) {
			--health;
			UpdateBar();

			const vec3f position = entities->GetComponent<Transform>(target)->GetWorldTranslation();

			Transform* const iTransform = indicatorLabel->Create();
			iTransform->translation = position;
			iTransform->translation.z = 5.f;

			Text* const text = entities->GetComponent<Text>(iTransform->entity);
			text->text = "-1";
			text->scale = 2.f;
			text->color = vec4f(1.f, 0.f, 0.f, 1.f);

			const vec3f direction = transform->GetWorldTranslation() - position;
			EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));

			inviT = inviDuration;
			ft = flashDuration;
			flashOverlay->tint.a = 1.f;

			if (health <= 0) EventsManager::Get()->Trigger("END_GAME");
		}
		break;
	default:
		break;
	}
}

void PlayerLife::Awake() {
	render = GetComponent<SpriteRender>();
}

void PlayerLife::Start() {
	health = maxHealth;
	inviT = -1.f;
	ft = 0.f;
	flashOverlay->tint.a = 0.f;

	UpdateBar();
}

void PlayerLife::Update() {
	if (inviT > 0.0f) {
		inviT -= time->dt;
		render->tint.a = render->tint.a > 0.5f ? 0.f : 1.f;

		if (inviT <= 0.f) {
			render->tint.a = 1.f;
		}
	} 

	if (ft > 0.f) {
		ft -= time->dt;
		
		if (ft <= 0.f) {
			flashOverlay->tint.a = 0.f; 
		}
	}
}

void PlayerLife::UpdateBar() {
	*barMultiplier = health / maxHealth;
	healthText->text = std::to_string(static_cast<int>(health)) + '/' + std::to_string(static_cast<int>(maxHealth));
}
