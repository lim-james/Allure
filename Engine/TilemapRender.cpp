#include "TilemapRender.h"

#include <Events/EventsManager.h>

TilemapRender::TilemapRender()
	: tint(1.f)
	, material(nullptr) {
}

void TilemapRender::Initialize() {
	tint = 1.f;
	SetMaterial(nullptr);
}

Component * TilemapRender::Clone() const {
	return new TilemapRender(*this);
}

void TilemapRender::SetActive(bool const& state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("TILEMAP_RENDER_ACTIVE", new Events::AnyType<TilemapRender*>(this));
}

void TilemapRender::SetDynamic(bool const & state) {
	Component::SetDynamic(state);
	EventsManager::Get()->Trigger("TILEMAP_RENDER_DYNAMIC", new Events::AnyType<TilemapRender*>(this));
}

Material::Base * const TilemapRender::GetMaterial() const {
	return material;
}

void TilemapRender::SetMaterial(Material::Base * _material) {
	Events::MaterialChange* event = new Events::MaterialChange(material, this);
	material = _material;
	EventsManager::Get()->Trigger("TILEMAP_MATERIAL", event);
}

TilemapPalette TilemapRender::GetPalette() const {
	return palette;
}

void TilemapRender::SetPalette(TilemapPalette const & newPalette) {
	Events::PaletteChange* event = new Events::PaletteChange(palette, this);
	palette = newPalette;
	EventsManager::Get()->Trigger("PALETTE_CHANGE", event);
}
