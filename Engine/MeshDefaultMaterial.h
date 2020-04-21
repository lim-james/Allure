#ifndef MESH_DEFAULT_MATERIAL_H
#define MESH_DEFAULT_MATERIAL_H

#include "Material.h"

namespace Material {
	struct MeshDefault: Base {
		bool useAlbedoMap;
		unsigned albedoMap;
		vec4f albedo;

		bool useNormalMap;
		unsigned normalMap;
		vec3f normal;

		bool useMetallicMap;
		unsigned metallicMap;
		float metallic;

		bool useRoughnessMap;
		unsigned roughnessMap;
		float roughness;

		bool useAOMap;
		unsigned aoMap;
		float ao;

		MeshDefault();
		void SetAttributes() override;
	};
}

#endif
