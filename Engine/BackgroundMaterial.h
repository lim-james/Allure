#ifndef BACKGROUND_MATERIAL_H
#define BACKGROUND_MATERIAL_H

#include "Material.h"

#include <vector>

namespace Material {
	struct Background: Base {

		float et;
		float speed;
		
		float spreadWeight;
		float spreadAlpha;
		struct Spread {
			float et;
			vec3f tint;
		};

		std::vector<Spread> spreads;

		float indicatorWeight;
		float indicatorRadius;
		float indicatorAlpha;
		vec3f indicatorTint;
		float viewSize;
		float interval;
		float bt;

		float thresholdWeight;
		float thresholdRadius;
		float thresholdAlpha;
		vec3f thresholdTint;
		float threshold;

		vec3f playerPosition;

		Background();

	private:

		void SetAttributes() override;

	};
}

#endif
