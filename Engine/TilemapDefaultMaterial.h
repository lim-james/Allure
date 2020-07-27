#ifndef TILEMAP_DEFAULT_MATERIAL_H
#define TILEMAP_DEFAULT_MATERIAL_H

#include "Material.h"

namespace Material {
	struct TilemapDefault : Base {
		TilemapDefault();
	private:
		void SetAttributes() override;
	};
}

#endif
