#include "TransformSystem.h"

#include <Events/EventsManager.h>
#include <Math/Mat4Transform.hpp>
#include <Logger/Logger.h>

TransformSystem::~TransformSystem() {
	delete dynamicTree;
	delete staticTree;
}

void TransformSystem::Initialize() {
	dynamicTree = new QuadTree<Transform*, TransformComparator>(5, 5);
	staticTree = new QuadTree<Transform*, TransformComparator>(5, 5);
	EventsManager::Get()->Subscribe("TRANSFORM_ACTIVE", &TransformSystem::ActiveHanlder, this);
	EventsManager::Get()->Subscribe("TRANSFORM_DYNAMIC", &TransformSystem::DynamicHanlder, this);
}

void TransformSystem::Update(float const& dt) {
	if (updateStatic) {
		UpdateTree(staticTree);
		updateStatic = false;
	}

	UpdateTree(dynamicTree);

	//tree->Clear();
	//tree->Sort();

	//system("cls");
	//Console::Warn << *tree << '\n';
}

void TransformSystem::ActiveHanlder(Events::Event * event) {
	auto component = static_cast<Events::AnyType<Transform*>*>(event)->data;
	auto tree = component->IsDynamic() ? dynamicTree : staticTree;

	if (component->IsActive()) {
		tree->Insert(component);
	} else {
		tree->Remove(component);
	}
}

void TransformSystem::DynamicHanlder(Events::Event * event) {
	auto component = static_cast<Events::AnyType<Transform*>*>(event)->data;
	if (!component->IsActive()) return;

	auto prev = component->IsDynamic() ? staticTree : dynamicTree;
	auto curr = component->IsDynamic() ? dynamicTree : staticTree;

	if (prev->Remove(component)) {
		updateStatic = true;
		curr->Insert(component);
	}
}

void TransformSystem::UpdateTree(QuadTree<Transform*, TransformComparator> const * tree) {
	for (auto& item : tree->GetRoot()->list) {
		if (item->parent == nullptr) 
			SetTransform(item);

		item->quads.clear();
		item->quads.push_back(tree->GetRoot());
	}
}

void TransformSystem::SetTransform(Transform * const transform) {
	Math::SetToTransform(
		transform->localTransform, 
		transform->translation, 
		transform->rotation, 
		transform->scale
	);

	if (transform->parent == nullptr) {
		transform->worldRotation = transform->rotation;
		transform->worldTranslation = transform->translation;

		transform->worldTransform = transform->localTransform;
	} else {
		auto const& worldTransform = transform->parent->worldTransform;

		transform->worldRotation = transform->rotation + transform->parent->worldRotation;
		transform->worldTranslation = worldTransform * transform->translation;

		Math::SetToTransform(
			transform->worldTransform,
			transform->worldTranslation,
			transform->worldRotation,
			transform->scale
		);
	}

	for (auto& child : transform->children) {
		SetTransform(child);
	}
}

