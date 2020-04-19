#include "Material.h"

#include <Events/EventsManager.h>

Material::Base::Base() : flags(0), shader(nullptr) {}

BitField const & Material::Base::GetFlags() const {
	return flags;
}

Shader * const Material::Base::GetShader() const {
	return shader;
}

void Material::Base::SetShader(Shader * const _shader) {
	auto event = new Events::ShaderChange(shader, this);
	shader = _shader;
	Events::EventsManager::GetInstance()->Trigger("MATERIAL_SHADER", event);
}

