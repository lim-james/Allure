#ifndef UNLIT_DEFAULT_MATERIAL_H
#define UNLIT_DEFAULT_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.h>

namespace Material {

	struct UnlitDefault : Base {

		float alphaClipping;

		unsigned texture;
		vec4f tint;

		vec2f tiling;
		vec2f offset;

		UnlitDefault();

	private:

		void SetAttributes() override;

	};

}

#endif
