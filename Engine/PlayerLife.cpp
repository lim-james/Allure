#include "PlayerLife.h"

#include "Layers.h"
#include "EnemyLife.h"
#include "Physics.h"
#include <Events/EventsManager.h>

void PlayerLife::OnCollisionEnter(unsigned target) {
	switch (entities->GetLayer(target)) {
	case ENEMY:
		if (inviT <= 0.f && entities->GetComponent<EnemyLife>(target)->bodyDamage) 
			Hit(target);
		break;
	case ENEMY_BULLET:
		if (inviT <= 0.f) 
			Hit(target);
		break;
	default:
		break;
	}
}

float PlayerLife::GetHealth() const {
	return health;
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

void PlayerLife::Hit(unsigned const& target) {
	--health;
	UpdateBar();

	const vec3f position = entities->GetComponent<Transform>(target)->GetWorldTranslation();
	const vec3f direction = transform->GetWorldTranslation() - position;
	EventsManager::Get()->Trigger("SCREEN_SHAKE", new Events::AnyType<vec2f>(direction));

	Transform* const iTransform = indicatorLabel->Create();
	iTransform->SetLocalTranslation(vec3f(position.xy, 5.f));

	Text* const text = entities->GetComponent<Text>(iTransform->entity);
	text->text = "-1";
	text->scale = 2.f;
	text->color = vec4f(1.f, 0.f, 0.f, 1.f);

	inviT = inviDuration;
	ft = flashDuration;
	flashOverlay->tint.a = 1.f;

	if (health <= 0) EventsManager::Get()->Trigger("END_GAME_D");
}

void PlayerLife::UpdateBar() {
	*barMultiplier = health / maxHealth;
	healthText->text = std::to_string(static_cast<int>(health)) + '/' + std::to_string(static_cast<int>(maxHealth));
}
