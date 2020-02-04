#include "SpriteAnimation.h"

#include <Events/EventsManager.h>

SpriteAnimation::SpriteAnimation()
	: currentAnimation("") {}

void SpriteAnimation::Initialize() {
	animations = {};
	currentAnimation = "";
}

void SpriteAnimation::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Queue("SPRITE_ANIMATION_ACTIVE", new Events::AnyType<SpriteAnimation*>(this));
}

