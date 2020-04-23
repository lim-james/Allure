#ifndef IBL_MATERIAL_H
#define IBL_MATERIAL_H

#include "Material.h"

namespace Material {
	struct IBL: Base {

		bool useAlbedoMap;
		unsigned albedoMap;
		vec4f albedo;

		bool useNormalMap;
		unsigned normalMap;

		bool useMetallicMap;
		unsigned metallicMap;
		float metallic;

		bool useRoughnessMap;
		unsigned roughnessMap;
		float roughness;

		bool useAOMap;
		unsigned aoMap;
		float ao;

		float refractiveIndex;
		unsigned environment;

		vec2f tiling;
		vec2f offset;

		IBL();

	private:

		void SetAttributes() override;
	};
}


#endif
