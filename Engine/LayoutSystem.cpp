#include "LayoutSystem.h"	

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

std::vector<std::function<float(Constraint const&)>> LayoutSystem::getters = {
	LayoutSystem::GetWidth,
	LayoutSystem::GetHeight,
	LayoutSystem::GetCenterX,
	LayoutSystem::GetCenterY,
	LayoutSystem::GetLeft,
	LayoutSystem::GetTop,
	LayoutSystem::GetRight,
	LayoutSystem::GetBottom,
	LayoutSystem::GetNA
};

std::vector<std::function<void(Transform* const, float const&, bool const&)>> LayoutSystem::setters = {
	LayoutSystem::SetWidth,
	LayoutSystem::SetHeight,
	LayoutSystem::SetCenterX,
	LayoutSystem::SetCenterY,
	LayoutSystem::SetLeft,
	LayoutSystem::SetTop,
	LayoutSystem::SetRight,
	LayoutSystem::SetBottom
};

void LayoutSystem::Initialize() {
	EventsManager::Get()->Subscribe("LAYOUT_ACTIVE", &LayoutSystem::ActiveHandler, this);
}

void LayoutSystem::Update(float const& dt) {
	for (Layout* const c : components) {
		Transform * const transform = entities->GetComponent<Transform>(c->entity);
		short warpX = 0;
		short warpY = 0;

		for (Constraint const& constraint : c->GetConstraints()) {
			const short i = constraint.self % 2;
			short& warp = i ? warpY : warpX;
			SetValue(transform, constraint.self, GetValue(constraint), warp);
			warp += constraint.self - i;
		}
	}
}

void LayoutSystem::ActiveHandler(Events::Event * event) {
	Layout* const c = static_cast<Events::AnyType<Layout*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(components, c);
	} else {
		Helpers::Remove(components, c);
	}
}

float LayoutSystem::GetValue(Constraint const& constraint) {
	return getters[constraint.other](constraint) * constraint.multiplier + constraint.constant;
}

float LayoutSystem::GetWidth(Constraint const& constraint) {
	return constraint.item 
		? constraint.item->GetScale().x
		: constraint.view->GetFrameSize().x * 2.f;
}

float LayoutSystem::GetHeight(Constraint const& constraint) {
	return constraint.item 
		? constraint.item->GetScale().y
		: constraint.view->GetFrameSize().y * 2.f;
}

float LayoutSystem::GetCenterX(Constraint const& constraint) {
	return constraint.item
		? constraint.item->GetWorldTranslation().x
		: 0.f;
}

float LayoutSystem::GetCenterY(Constraint const& constraint) {
	return constraint.item
		? constraint.item->GetWorldTranslation().y
		: 0.f;
}

float LayoutSystem::GetLeft(Constraint const& constraint) {
	return constraint.item
		? constraint.item->GetWorldTranslation().x - constraint.item->GetScale().x * 0.5f
		: -constraint.view->GetFrameSize().x;
}

float LayoutSystem::GetTop(Constraint const& constraint) {
	return constraint.item
		? constraint.item->GetWorldTranslation().y + constraint.item->GetScale().y * 0.5f
		: constraint.view->GetFrameSize().y;
}

float LayoutSystem::GetRight(Constraint const& constraint) {
	return constraint.item
		? constraint.item->GetWorldTranslation().x + constraint.item->GetScale().x * 0.5f
		: constraint.view->GetFrameSize().x;
}

float LayoutSystem::GetBottom(Constraint const& constraint) {
	return constraint.item
		? constraint.item->GetWorldTranslation().y - constraint.item->GetScale().y * 0.5f
		: -constraint.view->GetFrameSize().y;
}

float LayoutSystem::GetNA(Constraint const& constraint) {
	return 0.0f;
}

void LayoutSystem::SetValue(Transform * const transform, short const & attribute, float const & value, bool const& warp) {
	setters[attribute](transform, value, warp);
}

void LayoutSystem::SetWidth(Transform * const transform, float const & value, bool const& warp) {
	vec3f scale = transform->GetScale();
	scale.x = value;
	transform->SetScale(scale);
}

void LayoutSystem::SetHeight(Transform * const transform, float const & value, bool const& warp) {
	vec3f scale = transform->GetScale();
	scale.y = value;
	transform->SetScale(scale);
}

void LayoutSystem::SetCenterX(Transform * const transform, float const & value, bool const& warp) {
	vec3f translation = transform->GetScale();
	translation.x = value;
	transform->SetLocalTranslation(translation);
}

void LayoutSystem::SetCenterY(Transform * const transform, float const & value, bool const& warp) {
	vec3f translation = transform->GetScale();
	translation.y = value;
	transform->SetLocalTranslation(translation);
}

void LayoutSystem::SetLeft(Transform * const transform, float const & value, bool const& warp) {
	vec3f translation = transform->GetLocalTranslation();
	const float original = translation.x;
	translation.x = value + transform->GetScale().x * 0.5f;
	transform->SetLocalTranslation(translation);

	if (warp) {
		vec3f scale = transform->GetScale();
		scale.x += original - translation.x;
		transform->SetScale(scale);
	}
}

void LayoutSystem::SetTop(Transform * const transform, float const & value, bool const& warp) {
	vec3f translation = transform->GetLocalTranslation();
	const float original = translation.y;
	translation.y = value - transform->GetScale().y * 0.5f;
	transform->SetLocalTranslation(translation);

	if (warp) {
		vec3f scale = transform->GetScale();
		scale.y += translation.y - original;
		transform->SetScale(scale);
	}
}

void LayoutSystem::SetRight(Transform * const transform, float const & value, bool const& warp) {
	vec3f translation = transform->GetLocalTranslation();
	const float original = translation.x;
	translation.x = value - transform->GetScale().x * 0.5f;
	transform->SetLocalTranslation(translation);

	if (warp) {
		vec3f scale = transform->GetScale();
		scale.x += translation.x - original;
		transform->SetScale(scale);
	}
}

void LayoutSystem::SetBottom(Transform * const transform, float const & value, bool const& warp) {
	vec3f translation = transform->GetLocalTranslation();
	const float original = translation.y;
	translation.y = value + transform->GetScale().y * 0.5f;
	transform->SetLocalTranslation(translation);

	if (warp) {
		vec3f scale = transform->GetScale();
		scale.y += original - translation.y;
		transform->SetScale(scale);
	}
}
