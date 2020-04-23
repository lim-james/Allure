#ifndef PORTAL_MATERIAL_H
#define PORTAL_MATERIAL_H

#include "Material.h"

namespace Material {
	
	struct Portal : Base {

		unsigned texture;
		Portal();

	private:

		void SetAttributes() override;

	};

}

#endif
