#include "ChaseState.h"

#include "SpriteAnimation.h"
#include "EnemyTarget.h"

void States::Chase::Enter(unsigned const & target, EntityManager * const entities) {
	entities->GetComponent<SpriteAnimation>(target)->queued = "WALK";
}

void States::Chase::Update(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::Chase::FixedUpdate(unsigned const & target, float const & dt, EntityManager * const entities) {
	 entities->GetComponent<EnemyTarget>(target)->Chase();
}

void States::Chase::Exit(unsigned const & target, EntityManager * const entities) {}
