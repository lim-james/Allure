#ifndef MESH_RENDER_H
#define MESH_RENDER_H

#include "Component.h"
#include "Model.h"
#include "Material.h"

struct MeshRender : Component {


	MeshRender();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

	Material::Base* const GetMaterial() const;
	void SetMaterial(Material::Base* _material);

private:

	Material::Base* material;

};

#endif
