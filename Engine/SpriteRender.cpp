#include "SpriteRender.h"

#include "LoadTGA.h"

#include <Events/EventsManager.h>

SpriteRender::SpriteRender()
	: sprite(0)
	, uvRect(vec2f(0.f), vec2f(1.f))
	, tint(1.f)
	, tilemapUnit(1.f)
	, cellRect(vec2f(0.f), vec2f(1.f)) {
}

void SpriteRender::Initialize() {
	SetSprite(0);
	uvRect = vec4f(vec2f(0.f), vec2f(1.f));
	tint = vec4f(1.f);
	tilemapUnit = vec2f(1.f);
	cellRect = vec4f(vec2f(0.f), vec2f(1.f));
}

Component * SpriteRender::Clone() const {
	return new SpriteRender(*this);
}

void SpriteRender::SetActive(bool const& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("SPRITE_RENDER_ACTIVE", new Events::AnyType<SpriteRender*>(this));
}

unsigned const& SpriteRender::GetSprite() const {
	return sprite;
}

void SpriteRender::SetSprite(unsigned const& _sprite) {
	auto event = new Events::SpriteChange(sprite, this);
	sprite = _sprite;
	Events::EventsManager::GetInstance()->Trigger("SPRITE_CHANGE", event);
}

void SpriteRender::SetTilemapSize(int const& width, int const& height) {
	tilemapUnit.x = 1.f / static_cast<float>(width);
	tilemapUnit.y = 1.f / static_cast<float>(height);

	uvRect = cellRect * vec4f(tilemapUnit, tilemapUnit);
}

void SpriteRender::SetCellRect(int const& x, int const& y, int const& width, int const& height) {
	cellRect = vec4f(
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(width),
		static_cast<float>(height)
	);

	uvRect = cellRect * vec4f(tilemapUnit, tilemapUnit);
}
