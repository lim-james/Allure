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
	void SetDynamic(bool const& state) override;

	Model* const GetModel() const;
	void SetModel(Model* const _model);

	Material::Base* const GetMaterial() const;
	void SetMaterial(Material::Base* const _material);

private:

	Model* model;
	Material::Base* material;

};

namespace Events {
	struct ModelChange : Event {
		Model * const previous;
		MeshRender * const component;

		ModelChange(Model * const previous, MeshRender * const component)
			: previous(previous)
			, component(component) {}
	};
}

#endif
