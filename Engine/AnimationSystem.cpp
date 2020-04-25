#include "AnimationSystem.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

void AnimationSystem::Initialize() {
	Events::EventsManager::GetInstance()->Subscribe("ANIMATION_ACTIVE", &AnimationSystem::ActiveHandler, this);
}

void AnimationSystem::Update(float const& dt) {
	for (Animation* const c : components) {
		auto& anims = c->animations;

		if (anims.empty()) continue;

		std::vector<int> keys;

		for (auto& a : anims) {
			keys.push_back(a.first);
			a.second->Update(static_cast<float>(dt));
		}

		const int size = static_cast<int>(keys.size());

		for (int i = size - 1; i >= 0; --i) {
			int const& key = keys[i];
			if (!anims[key]->IsActive()) {
				delete anims[key];
				anims.erase(key);
			}
		}
	}
}

void AnimationSystem::ActiveHandler(Events::Event * event) {
	Animation* c = static_cast<Events::AnyType<Animation*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(components, c);
	} else {
		Helpers::Remove(components, c);
	}
}
