#include "Material.h"

#include <Events/EventsManager.h>
#include <GL/glew.h>

Material::Base::Base() : flags(CULL_FACE), shader(nullptr) {}

BitField const & Material::Base::GetFlags() const {
	return flags;
}

Shader * const Material::Base::GetShader() const {
	return shader;
}

void Material::Base::SetShader(Shader * const _shader) {
	auto event = new Events::ShaderChange(shader, this);
	shader = _shader;
	EventsManager::Get()->Trigger("MATERIAL_SHADER", event);
}

void Material::Base::Use() {
	if (flags == CULL_FACE) {
		glEnable(GL_CULL_FACE);
		glCullFace(flags == FRONT_FACE ? GL_FRONT : GL_BACK);
	} else {
		glDisable(GL_CULL_FACE);
	}

	SetAttributes();
}

