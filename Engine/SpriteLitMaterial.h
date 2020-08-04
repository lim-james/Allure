#ifndef SPRITE_LIT_MATERIAL_H
#define SPRITE_LIT_MATERIAL_H

#include "Material.h"

namespace Material {
	struct SpriteLit : Base {
		SpriteLit();
	private:
		void SetAttributes() override;
	};
}

#endif
