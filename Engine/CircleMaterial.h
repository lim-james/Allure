#ifndef CIRCLE_MATERIAL_H
#define CIRCLE_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.h>
#include <vector>

namespace Material {
	struct Circle : Base {

		float borderWeight;

		Circle();

	private:

		void SetAttributes() override;

	};
}

#endif