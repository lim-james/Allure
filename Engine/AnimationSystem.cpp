#include "AnimationSystem.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

void AnimationSystem::Initialize() {
	EventsManager::Get()->Subscribe("ANIMATION_ACTIVE", &AnimationSystem::ActiveHandler, this);
}

void AnimationSystem::Update(float const& dt) {
	for (Animation* const c : components) {
		auto& anims = c->animations;

		if (anims.empty()) continue;

		std::vector<int> keys;

		for (auto& a : anims) {
			keys.push_back(a.first);
			a.second[0]->Update(static_cast<float>(dt));
		}

		const int size = static_cast<int>(keys.size());

		for (int i = size - 1; i >= 0; --i) {
			int const& key = keys[i];
			auto& list = anims[key];

			if (!list[0]->IsActive()) {
				delete list[0];
				list.erase(list.begin());
				if (list.empty())
					anims.erase(key);
			}
		}
	}
}

void AnimationSystem::ActiveHandler(Events::Event * event) {
	Animation* const c = static_cast<Events::AnyType<Animation*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(components, c);
	} else {
		Helpers::Remove(components, c);
	}
}
