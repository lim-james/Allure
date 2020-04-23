#ifndef SKYBOX_MATERIAL_H
#define SKYBOX_MATERIAL_H

#include "Material.h"

namespace Material {

	struct Skybox : Base {

		unsigned cubemap;
		Skybox();

	private:

		void SetAttributes() override;

	};

}

#endif
