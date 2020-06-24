#ifndef SPECTRUM_BUBBLE_MATERIAL_H
#define SPECTRUM_BUBBLE_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.h>
#include <vector>

namespace Material {
	struct SpectrumBubble : Base {

		float minRadius;
		float maxValue;
		float unit;
		float multiplier;
	
		float outlineWeight;
		vec3f outlineColor;

		std::vector<vec2d> values;

		SpectrumBubble();

	private:

		void SetAttributes() override;

	};
}

#endif
