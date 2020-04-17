#ifndef MESH_DEFAULT_MATERIAL_H
#define MESH_DEFAULT_MATERIAL_H

#include "Material.h"

namespace Material {
	struct MeshDefault: Base {
		MeshDefault();
		void SetAttributes() override;
	};
}

#endif
