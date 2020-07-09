#include "BeatNeutralState.h"

#include "SpriteRender.h"
#include "Physics.h"

void States::BeatNeutral::Enter(unsigned const & target, EntityManager * const entities) {
	SpriteRender* const render = entities->GetComponent<SpriteRender>(target);
	render->SetMaterial(nullptr);

	Physics* const physics = entities->GetComponent<Physics>(target);
	physics->SetActive(true);
}

void States::BeatNeutral::Update(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::BeatNeutral::FixedUpdate(unsigned const & target, float const & dt, EntityManager * const entities) {}

void States::BeatNeutral::Exit(unsigned const & target, EntityManager * const entities) {
	Physics* const physics = entities->GetComponent<Physics>(target);
	physics->SetActive(false);
}
