#include "VoxelRender.h"

#include <Events/EventsManager.h>

VoxelRender::VoxelRender() 
	: tint(1.f)
	, material(nullptr) {
}

void VoxelRender::Initialize() {
	tint = 1.0f;
	SetMaterial(nullptr);
}

Component * VoxelRender::Clone() const {
	return new VoxelRender(*this);
}

void VoxelRender::SetActive(bool const& state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("VOXEL_RENDER_ACTIVE", new Events::AnyType<VoxelRender*>(this));
}

void VoxelRender::SetDynamic(bool const & state) {
	Component::SetDynamic(state);
	EventsManager::Get()->Trigger("VOXEL_RENDER_DYNAMIC", new Events::AnyType<VoxelRender*>(this));
}

Material::Base * const VoxelRender::GetMaterial() const {
	return material;
}

void VoxelRender::SetMaterial(Material::Base * _material) {
	Events::MaterialChange* event = new Events::MaterialChange(material, this);
	material = _material;
	EventsManager::Get()->Trigger("VOXEL_MATERIAL", event);
}