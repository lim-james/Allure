#ifndef VOXEL_RENDER_H
#define VOXEL_RENDER_H

#include "Component.h"
#include "Material.h"

#include <Math/Vectors.h>

struct VoxelRender : Component {

	// size of UV in tilemap
	vec4f tint;

	VoxelRender();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;
	void SetDynamic(bool const& state) override;

	Material::Base* const GetMaterial() const;
	void SetMaterial(Material::Base* _material);

private:

	Material::Base* material;

};

#endif
