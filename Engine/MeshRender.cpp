#include "MeshRender.h"

#include <Events/EventsManager.h>

MeshRender::MeshRender() 
	: mesh(nullptr)
	, material(nullptr) {
}

void MeshRender::Initialize() {
	mesh = nullptr;
	SetMaterial(nullptr);
}

Component * MeshRender::Clone() const {
	return new MeshRender(*this);
}

void MeshRender::SetActive(bool const & state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("MESH_RENDER_ACTIVE", new Events::AnyType<MeshRender*>(this));
}

void MeshRender::SetDynamic(bool const & state) {
	Component::SetDynamic(state);
	EventsManager::Get()->Trigger("MESH_RENDER_DYNAMIC", new Events::AnyType<MeshRender*>(this));
}

Mesh * const MeshRender::GetMesh() const {
	return mesh;
}

void MeshRender::SetMesh(Mesh * const _mesh) {
	Events::MeshChange* event = new Events::MeshChange(mesh, this);
	mesh = _mesh;
	EventsManager::Get()->Trigger("MESH_CHANGE", event);
}

Material::Base * const MeshRender::GetMaterial() const {
	return material;
}

void MeshRender::SetMaterial(Material::Base * const _material) {
	Events::MaterialChange* event = new Events::MaterialChange(material, this);
	material = _material;
	EventsManager::Get()->Trigger("MESH_MATERIAL", event);
}

bool MeshRender::IsCastingShadow() const {
	return castShadow;
}

void MeshRender::SetCastShader(bool const & state) {
	castShadow = state;
	EventsManager::Get()->Trigger("CAST_SHADOW", new Events::AnyType<MeshRender*>(this));
}
