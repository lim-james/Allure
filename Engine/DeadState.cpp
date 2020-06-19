#include "DeadState.h"

#include "Physics.h"
#include "SphereCollider.h"
#include "SpriteAnimation.h"
#include "SpriteRender.h"
#include "EnemyLife.h"

void States::Dead::Enter(unsigned const & target, EntityManager * const entities) {
	entities->GetComponent<SphereCollider>(target)->SetActive(false);
	entities->GetComponent<SpriteAnimation>(target)->queued = "DEAD";
}

void States::Dead::Update(unsigned const & target, float const & dt, EntityManager * const entities) {
	SpriteRender* const render = entities->GetComponent<SpriteRender>(target);
	render->tint.a -= dt;
	if (render->tint.a < 0.f) {
		entities->GetComponent<EnemyLife>(target)->Kill();
	}
}

void States::Dead::FixedUpdate(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::Dead::Exit(unsigned const & target, EntityManager * const entities) {}
