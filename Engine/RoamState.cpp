#include "RoamState.h"

// components
#include "Transform.h"
#include "Render.h"
#include "StateContainer.h"

#include "AIAttribute.h"

#include <Math/Random.hpp>
#include <Logger/Logger.h>

namespace States {

	void RoamState::Enter(const unsigned& target, EntityManager * const entities) {
		//Console::Warn << target << " roaming\n";
	}

	void RoamState::Update(const unsigned& target, const float& dt, EntityManager * const entities) {
		auto transform = entities->GetComponent<Transform>(target);
		auto ai = entities->GetComponent<AIAttribute>(target);
		auto d = ai->destination - transform->translation;
		const auto length = Math::Length(d);

		const float mag = ai->speed * dt;

		if (length <= mag) {
			transform->translation = ai->destination;
			ai->destination = transform->translation;
			if (Math::RandSign() > 0) {
				ai->destination.x += static_cast<float>(Math::RandSign());
			} else {
				ai->destination.y += static_cast<float>(Math::RandSign());
			}
		} else {
			transform->translation += (d / length) * mag;
		}

		transform->translation.x = Math::Min(Math::Max(-9.5f, transform->translation.x), 9.5f);
		transform->translation.y = Math::Min(Math::Max(-9.5f, transform->translation.y), 9.5f);
	}

	void RoamState::Exit(const unsigned& target, EntityManager * const entities) {
			
	}

}