#ifndef VOXEL_DEFAULT_MATERIAL_H
#define VOXEL_DEFAULT_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.h>

namespace Material {

	struct VoxelDefault : Base {
		VoxelDefault();
	private: 
		void SetAttributes() override;
	};

}

#endif
