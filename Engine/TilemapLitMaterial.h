#ifndef TILEMAP_LIT_MATERIAL_H
#define TILEMAP_LIT_MATERIAL_H

#include "Material.h"

namespace Material {
	struct TilemapLit : Base {
		TilemapLit();
	private:
		void SetAttributes() override;
	};
}

#endif
