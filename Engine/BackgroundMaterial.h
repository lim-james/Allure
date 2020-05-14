#ifndef BACKGROUND_MATERIAL_H
#define BACKGROUND_MATERIAL_H

#include "Material.h"

namespace Material {
	struct Background: Base {

		float et;
		float speed;
		float spread;
		vec3f spreadTint;

		vec3f indicatorTint;
		float viewSize;
		float interval;
		float bt;

		vec3f thresholdTint;
		float threshold;

		vec3f playerPosition;

		Background();

	private:

		void SetAttributes() override;

	};
}

#endif
