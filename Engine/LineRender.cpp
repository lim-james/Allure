#include "LineRender.h"

#include <Events/EventsManager.h>

LineRender::LineRender()
	: offset(0.f)
	, length(1.f)
	, tint(1.f)
	, material(nullptr) {
}

void LineRender::Initialize() {
	offset = vec3f(0.f);
	length = vec3f(1.f);
	tint = vec4f(1.f);
	material = nullptr;
}

Component* LineRender::Clone() const {
	return new LineRender(*this);
}

void LineRender::SetActive(bool const& state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("LINE_RENDER_ACTIVE", new Events::AnyType<LineRender*>(this));
}

void LineRender::SetDynamic(bool const & state) {
	Component::SetDynamic(state);
	EventsManager::Get()->Trigger("LINE_RENDER_DYNAMIC", new Events::AnyType<LineRender*>(this));
}

Material::Base * const LineRender::GetMaterial() const {
	return material;
}

void LineRender::SetMaterial(Material::Base * _material) {
	auto event = new Events::MaterialChange(material, this);
	material = _material;
	EventsManager::Get()->Trigger("LINE_MATERIAL", event);
}
