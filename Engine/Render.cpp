#include "Render.h"

#include "LoadTGA.h"

#include <Events/EventsManager.h>

Render::Render()
	: texture(0)
	, uvRect(vec2f(0.f), vec2f(1.f))
	, tint(1.f)
	, tilemapUnit(1.f)
	, cellRect(vec2f(0.f), vec2f(1.f)) {
}

void Render::Initialize() {
	SetTexture(0);
	uvRect = vec4f(vec2f(0.f), vec2f(1.f));
	tint = vec4f(1.f);
	tilemapUnit = vec2f(1.f);
	cellRect = vec4f(vec2f(0.f), vec2f(1.f));
}

Component * Render::Clone() const {
	return new Render(*this);
}

void Render::SetActive(bool const& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("RENDER_ACTIVE", new Events::AnyType<Render*>(this));
}

unsigned const& Render::GetTexture() const {
	return texture;
}

void Render::SetTexture(unsigned const& _texture) {
	auto event = new Events::TextureChange(texture, this);
	texture = _texture;
	Events::EventsManager::GetInstance()->Trigger("TEXTURE_CHANGE", event);
}

void Render::SetTexture(std::string const& _texture) {
	auto event = new Events::TextureChange(texture, this);
	texture = Load::TGA(_texture);
	Events::EventsManager::GetInstance()->Trigger("TEXTURE_CHANGE", event);

}

void Render::SetTilemapSize(int const& width, int const& height) {
	tilemapUnit.x = 1.f / static_cast<float>(width);
	tilemapUnit.y = 1.f / static_cast<float>(height);

	uvRect = cellRect * vec4f(tilemapUnit, tilemapUnit);
}

void Render::SetCellRect(int const& x, int const& y, int const& width, int const& height) {
	cellRect = vec4f(
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(width),
		static_cast<float>(height)
	);

	uvRect = cellRect * vec4f(tilemapUnit, tilemapUnit);
}
