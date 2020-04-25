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
	Events::EventsManager::GetInstance()->Subscribe("LAYOUT_ACTIVE", &LayoutSystem::ActiveHandler, this);
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
		? constraint.item->scale.x
		: constraint.view->GetFrameSize().x * 2.f;
}

float LayoutSystem::GetHeight(Constraint const& constraint) {
	return constraint.item 
		? constraint.item->scale.y
		: constraint.view->GetFrameSize().y * 2.f;
}

float LayoutSystem::GetCenterX(Constraint const& constraint) {
	return constraint.item
		? constraint.item->translation.x
		: 0.f;
}

float LayoutSystem::GetCenterY(Constraint const& constraint) {
	return constraint.item
		? constraint.item->translation.y
		: 0.f;
}

float LayoutSystem::GetLeft(Constraint const& constraint) {
	return constraint.item
		? constraint.item->translation.x - constraint.item->scale.x * 0.5f
		: -constraint.view->GetFrameSize().x;
}

float LayoutSystem::GetTop(Constraint const& constraint) {
	return constraint.item
		? constraint.item->translation.y + constraint.item->scale.y * 0.5f
		: constraint.view->GetFrameSize().y;
}

float LayoutSystem::GetRight(Constraint const& constraint) {
	return constraint.item
		? constraint.item->translation.x + constraint.item->scale.x * 0.5f
		: constraint.view->GetFrameSize().x;
}

float LayoutSystem::GetBottom(Constraint const& constraint) {
	return constraint.item
		? constraint.item->translation.y - constraint.item->scale.y * 0.5f
		: -constraint.view->GetFrameSize().y;
}

float LayoutSystem::GetNA(Constraint const& constraint) {
	return 0.0f;
}

void LayoutSystem::SetValue(Transform * const transform, short const & attribute, float const & value, bool const& warp) {
	setters[attribute](transform, value, warp);
}

void LayoutSystem::SetWidth(Transform * const transform, float const & value, bool const& warp) {
	transform->scale.x = value;
}

void LayoutSystem::SetHeight(Transform * const transform, float const & value, bool const& warp) {
	transform->scale.y = value;
}

void LayoutSystem::SetCenterX(Transform * const transform, float const & value, bool const& warp) {
	transform->translation.x = value;
}

void LayoutSystem::SetCenterY(Transform * const transform, float const & value, bool const& warp) {
	transform->translation.y = value;
}

void LayoutSystem::SetLeft(Transform * const transform, float const & value, bool const& warp) {
	const float original = transform->translation.x;
	transform->translation.x = value + transform->scale.x * 0.5f;
	if (warp) transform->scale.x += original - transform->translation.x;
}

void LayoutSystem::SetTop(Transform * const transform, float const & value, bool const& warp) {
	const float original = transform->translation.y;
	transform->translation.y = value - transform->scale.y * 0.5f;
	if (warp) transform->scale.y += transform->translation.y - original;
}

void LayoutSystem::SetRight(Transform * const transform, float const & value, bool const& warp) {
	const float original = transform->translation.x;
	transform->translation.x = value - transform->scale.x * 0.5f;
	if (warp) transform->scale.x += transform->translation.x - original;
}

void LayoutSystem::SetBottom(Transform * const transform, float const & value, bool const& warp) {
	const float original = transform->translation.y;
	transform->translation.y = value + transform->scale.y * 0.5f;
	if (warp) transform->scale.y += original - transform->translation.y;
}
