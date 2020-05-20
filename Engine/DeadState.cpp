#include "DeadState.h"

#include "Physics.h"
#include "SphereCollider.h"
#include "SpriteAnimation.h"
#include "SpriteRender.h"

void States::Dead::Enter(unsigned const & target, EntityManager * const entities) {
	entities->GetComponent<Physics>(target)->SetActive(false);
	entities->GetComponent<SphereCollider>(target)->SetActive(false);
	entities->GetComponent<SpriteAnimation>(target)->queued = "DEAD";
}

void States::Dead::Update(unsigned const & target, float const & dt, EntityManager * const entities) {
	entities->GetComponent<SpriteRender>(target)->tint.a -= dt;
}

void States::Dead::FixedUpdate(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::Dead::Exit(unsigned const & target, EntityManager * const entities) {}
