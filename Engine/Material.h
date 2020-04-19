#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

#include "Component.h"
#include <Bit/BitField.h>
#include <Events/Event.h>

#define SURFACE_TRANSPARENT 0b1

namespace Material {

	struct Base {

		BitField flags;

		Base();

		BitField const& GetFlags() const;

		Shader* const GetShader() const;
		void SetShader(Shader* const _shader);

		virtual void SetAttributes() = 0;

	protected:

		Shader* shader;

	};

}

namespace Events {
	struct ShaderChange : Event {
		Shader* const previous;
		Material::Base* const material;

		ShaderChange(Shader* const previous, Material::Base* const material)
			: previous(previous)
			, material(material) {}
	};

	struct MaterialChange : Event {
		Material::Base* const previous;
		Component * const component;

		MaterialChange(Material::Base* const previous, Component * const component)
			: previous(previous)
			, component(component) {}
	};

}

#endif
