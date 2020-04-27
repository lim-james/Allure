#ifndef MESH_RENDER_H
#define MESH_RENDER_H

#include "Component.h"
#include "Mesh.h"
#include "Material.h"

struct MeshRender : Component {

	MeshRender();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;
	void SetDynamic(bool const& state) override;

	Mesh* const GetMesh() const;
	void SetMesh(Mesh* const _mesh);

	Material::Base* const GetMaterial() const;
	void SetMaterial(Material::Base* const _material);

	bool IsCastingShadow() const;
	void SetCastShader(bool const& state);

private:

	Mesh* mesh;
	Material::Base* material;

	bool castShadow;

};

namespace Events {
	struct MeshChange : Event {
		Mesh * const previous;
		MeshRender * const component;

		MeshChange(Mesh * const previous, MeshRender * const component)
			: previous(previous)
			, component(component) {}
	};
}

#endif
