#include "MeshRender.h"

#include <Events/EventsManager.h>

MeshRender::MeshRender() 
	: model(nullptr)
	, material(nullptr) {
}

void MeshRender::Initialize() {
	model = nullptr;
	SetMaterial(nullptr);
}

Component * MeshRender::Clone() const {
	return new MeshRender(*this);
}

void MeshRender::SetActive(bool const & state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("MESH_RENDER_ACTIVE", new Events::AnyType<MeshRender*>(this));
}

void MeshRender::SetDynamic(bool const & state) {
	Component::SetDynamic(state);
	Events::EventsManager::GetInstance()->Trigger("MESH_RENDER_DYNAMIC", new Events::AnyType<MeshRender*>(this));
}

Model * const MeshRender::GetModel() const {
	return model;
}

void MeshRender::SetModel(Model * const _model) {
	Events::ModelChange* event = new Events::ModelChange(model, this);
	model = _model;
	Events::EventsManager::GetInstance()->Trigger("MODEL_CHANGE", event);
}

Material::Base * const MeshRender::GetMaterial() const {
	return material;
}

void MeshRender::SetMaterial(Material::Base * const _material) {
	Events::MaterialChange* event = new Events::MaterialChange(material, this);
	material = _material;
	Events::EventsManager::GetInstance()->Trigger("MESH_MATERIAL", event);
}
