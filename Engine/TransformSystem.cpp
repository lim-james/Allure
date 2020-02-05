#include "TransformSystem.h"

#include <Events/EventsManager.h>
#include <Math/Mat4Transform.hpp>
#include <Logger/Logger.h>

TransformSystem::~TransformSystem() {
	delete tree;
}

void TransformSystem::Initialize() {
	tree = new QuadTree<Transform*, TransformComparator>(5, 5);
	Events::EventsManager::GetInstance()->Subscribe("TRANSFORM_ACTIVE", &TransformSystem::ActiveHanlder, this);
}

void TransformSystem::Update(const float & dt) {
	std::vector<Transform*> children;

	for (auto& item : tree->GetRoot()->list) {
		if (item->parent == nullptr) {
			SetTransform(item);
		}
		item->quads.clear();
		item->quads.push_back(tree->GetRoot());
	}

	//tree->Clear();
	//tree->Sort();

	//system("cls");
	//Console::Warn << *tree << '\n';
}

void TransformSystem::ActiveHanlder(Events::Event * event) {
	auto component = static_cast<Events::AnyType<Transform*>*>(event)->data;
	if (component->IsActive()) {
		tree->Insert(component);
	} else {
		tree->Remove(component);
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
		const auto& worldTransform = transform->parent->worldTransform;

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

