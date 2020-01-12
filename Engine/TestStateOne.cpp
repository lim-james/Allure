#include "TestStateOne.h"

#include "Transform.h"
#include "Render.h"
#include "StateContainer.h"

#include <Logger/Logger.h>

namespace States {

	void TestStateOne::Enter(const unsigned& target, EntityManager * const entities) {
		entities->GetComponent<Render>(target)->tint.Set(1.f, 0.f, 0.f, 1.f);
	}

	void TestStateOne::Update(const unsigned& target, const float& dt, EntityManager * const entities) {
		auto transform = entities->GetComponent<Transform>(target);
		if (transform->translation.x > 5.f) {
			entities->GetComponent<StateContainer>(target)->queuedState = "TWO";
		} else {
			transform->translation.x += 10.f * dt;
		}
	}

	void TestStateOne::Exit(const unsigned& target, EntityManager * const entities) {
	
	}

}