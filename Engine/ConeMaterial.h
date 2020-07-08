#ifndef CONE_MATERIAL_H
#define CONE_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.h>
#include <vector>

namespace Material {
	struct Cone : Base {

		float borderWeight;

		Cone();

		void SetRange(float const& deg);

	private:

		float range;

		void SetAttributes() override;

	};
}

#endif
