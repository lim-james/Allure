#include "AnimationSystem.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

void AnimationSystem::Initialize() {
	Events::EventsManager::GetInstance()->Subscribe("ANIMATION_ACTIVE", &AnimationSystem::ActiveHandler, this);
}

void AnimationSystem::Update(const float & dt) {
	for (auto& c : components) {
		std::vector<AnimationBase*>& anims = c->animations;

		if (!anims.size()) continue;

		for (auto& a : anims)
			a->Update(static_cast<float>(dt));

		const int size = static_cast<int>(anims.size());

		for (int i = size - 1; i >= 0; --i) {
			if (!c->animations[i]->IsActive()) {
				delete anims[i];
				anims.erase(anims.begin() + i);
			}
		}
	}
}

void AnimationSystem::ActiveHandler(Events::Event * event) {
	const auto c = static_cast<Events::AnyType<Animation*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(components, c);
	} else {
		Helpers::Remove(components, c);
	}
}
