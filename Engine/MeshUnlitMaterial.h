#ifndef MESH_UNLIT_MATERIAL_H
#define MESH_UNLIT_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.h>

namespace Material {

	struct MeshUnlit : Base {

		float alphaClipping;

		unsigned texture;
		vec4f tint;

		vec2f tiling;
		vec2f offset;

		MeshUnlit();

	private:

		void SetAttributes() override;

	};

}

#endif
