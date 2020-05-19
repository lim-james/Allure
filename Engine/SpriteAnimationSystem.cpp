#include "SpriteAnimationSystem.h"

#include "SpriteRender.h"

#include <Events/EventsManager.h>

SpriteAnimationSystem::~SpriteAnimationSystem() {
	components.clear();
}

void SpriteAnimationSystem::Initialize() {
	EventsManager::Get()->Subscribe("SPRITE_ANIMATION_ACTIVE", &SpriteAnimationSystem::ActiveHandler, this);
}

void SpriteAnimationSystem::Update(float const& dt) {
	for (auto& c : components) {
		if (c->queued != "" && c->queued != c->currentAnimation) {
			ResetAnimation(c->animations[c->currentAnimation]);
			c->currentAnimation = c->queued;
			c->queued = "";
		}

		if (c->currentAnimation != "") {
			auto& currentAnimation = c->animations[c->currentAnimation];
			currentAnimation.et += dt;
			ProcessAnimation(c, currentAnimation);
		}
	}
}

void SpriteAnimationSystem::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<SpriteAnimation*>*>(event)->data;

	if (c->IsActive()) {
		components.push_back(c);
	} else {
		components.erase(vfind(components, c));
	}
}

void SpriteAnimationSystem::ResetAnimation(SpriteAnimationData& animation) {
	animation.et = 0.f;
	animation.currentFrame = 0;
}

void SpriteAnimationSystem::ProcessAnimation(SpriteAnimation* component, SpriteAnimationData& animation) {
	if (animation.frames.size() <= animation.currentFrame) return;

	auto& currentFrame = animation.frames[animation.currentFrame];
	if (animation.et >= currentFrame.duration) {
		++animation.currentFrame;
		if (animation.currentFrame == animation.frames.size()) {
			if (animation.loop) {
				animation.currentFrame = 0;
			} else {
				animation.completed();
				return;
			}
		}
		animation.et = 0.f;
		SetProperties(component, animation);
	}
}

void SpriteAnimationSystem::SetProperties(SpriteAnimation* component, SpriteAnimationData const& animation) {
	auto render = entities->GetComponent<SpriteRender>(component->entity);
	render->uvRect = animation.frames[animation.currentFrame].uvRect;
}
