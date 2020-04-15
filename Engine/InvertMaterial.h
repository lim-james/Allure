#ifndef INVERT_MATERIAL_H
#define INVERT_MATERIAL_H

#include "Material.h"

namespace Material {
	struct Invert: Base {
		Invert();
		void SetAttributes() override;
	};
}

#endif
