#ifndef VOXEL_UNLIT_MATERIAL_H
#define VOXEL_UNLIT_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.h>

namespace Material {

	struct VoxelUnlit : Base {
		VoxelUnlit();
	private:
		void SetAttributes() override;
	};

}

#endif
