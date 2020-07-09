#include "AnimationSystem.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

void AnimationSystem::Initialize() {
	EventsManager::Get()->Subscribe("ANIMATOR_ACTIVE", &AnimationSystem::BaseActiveHandler, this);
	EventsManager::Get()->Subscribe("TRANSFORM_ANIMATOR_ACTIVE", &AnimationSystem::TransformActiveHandler, this);
}

void AnimationSystem::Update(float const& dt) {
	for (Animator* const c : base) {
		auto& anims = c->animations;

		if (anims.empty()) continue;

		std::vector<int> keys;

		for (auto& a : anims) {
			keys.push_back(a.first);
			a.second[0]->Update(dt);
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

	for (TransformAnimator* const c : transforms) {
		Transform* const transform = entities->GetComponent<Transform>(c->entity);

		UpdateTransform(c->animations[ANIMATE_SCALE], transform, &Transform::GetScale, &Transform::SetScale, dt);
		UpdateTransform(c->animations[ANIMATE_ROTATION], transform, &Transform::GetLocalRotation, &Transform::SetLocalRotation, dt);
		UpdateTransform(c->animations[ANIMATE_TRANSLATION], transform, &Transform::GetLocalTranslation, &Transform::SetLocalTranslation, dt);
	}
}

void AnimationSystem::BaseActiveHandler(Events::Event * event) {
	Animator* const c = static_cast<Events::AnyType<Animator*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(base, c);
	} else {
		Helpers::Remove(base, c);
	}
}

void AnimationSystem::TransformActiveHandler(Events::Event * event) {
	TransformAnimator* const c = static_cast<Events::AnyType<TransformAnimator*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(transforms, c);
	} else {
		Helpers::Remove(transforms, c);
	}
}

void AnimationSystem::UpdateTransform(
	std::vector<TransformAnimationData>& animations, 
	Transform* const transform, 
	vec3f const& (Transform::*getter)() const, 
	void(Transform::*setter)(vec3f const&), 
	float const& dt
) {
	if (animations.empty()) return;

	auto& base = animations[0];

	if (base.et == 0.f) {
		base.original = (transform->*getter)();
	}

	base.Update(dt);
	
	if (base.et >= base.duration) {
		if (base.loop) {
			(transform->*setter)(base.original);
			base.et = 0.f;
		} else {
			(transform->*setter)(base.outcome);
			base.completion.Invoke();
			animations.erase(animations.begin());
		}
	} else if (base.et > 0.f) {
		const vec3f current = (transform->*getter)();
		const vec3f interval = base.outcome - current;
		const float ratio = dt / (base.duration - base.et);

		(transform->*setter)(current + interval * ratio);
	}
}
