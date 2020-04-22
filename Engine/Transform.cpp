#include "Transform.h"

#include <Math/Mat4Transform.hpp>
#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

Transform::Transform() 
	: parent(nullptr)
	, translation(1.f) 
	, rotation(0.f) 
	, scale(0.f) {
	UpdateAxes();
}

void Transform::Initialize() {
	SetParent(nullptr);

	for (int i = static_cast<int>(children.size()) - 1; i >= 0; --i)
		children[i]->SetParent(nullptr);
	children.clear();

	translation = vec3f(0.f);
	rotation = vec3f(0.f);
	scale = vec3f(1.f);

	UpdateAxes();
}

void Transform::SetActive(bool const& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("TRANSFORM_ACTIVE", new Events::AnyType<Transform*>(this));
}

void Transform::SetDynamic(bool const & state) {
	Component::SetDynamic(state);
	Events::EventsManager::GetInstance()->Trigger("TRANSFORM_DYNAMIC", new Events::AnyType<Transform*>(this));
}

Component * Transform::Clone() const {
	return new Transform(*this);
}

void Transform::Unpack(AENotation const& data) {
	Component::Unpack(data);
	translation = data.Get<vec3f>("translation");
	rotation = data.Get<vec3f>("rotation");
	scale = data.Get<vec3f>("scale");
}

void Transform::Pack(AENotation& data) {
	data.Set("type", "Transform");
	Component::Pack(data);
	data.Set("translation", translation);
	data.Set("rotation", rotation);
	data.Set("scale", scale);
}

void Transform::UpdateAxes() {
	const float yawRad = rotation.y * Math::toRad;
	const float pitchRad = -rotation.x * Math::toRad;
	const float rollRad = rotation.z * Math::toRad;

	localAxes.z.z = -cos(yawRad) * cos(pitchRad);
	localAxes.z.y = sin(pitchRad);
	localAxes.z.x = sin(yawRad) * cos(pitchRad);

	const vec3f worldUp(sin(rollRad), cos(rollRad), 0.f);

	localAxes.x = Math::Normalized(Math::Cross(localAxes.z, worldUp));
	localAxes.y = Math::Normalized(Math::Cross(localAxes.x, localAxes.z));

	if (parent) {
		const float yawRad = worldRotation.y * Math::toRad;
		const float pitchRad = -worldRotation.x * Math::toRad;
		const float rollRad = worldRotation.z * Math::toRad;

		worldAxes.z.z = -cos(yawRad) * cos(pitchRad);
		worldAxes.z.y = sin(pitchRad);
		worldAxes.z.x = sin(yawRad) * cos(pitchRad);

		const vec3f worldUp(sin(rollRad), cos(rollRad), 0.f);

		worldAxes.x = Math::Normalized(Math::Cross(worldAxes.z, worldUp));
		worldAxes.y = Math::Normalized(Math::Cross(worldAxes.x, worldAxes.z));
	} else {
		worldAxes = localAxes;
	}

	for (auto& child : children) {
		child->UpdateAxes();
	}
}

vec3f const& Transform::GetLocalUp() const {
	return localAxes.y;
}

vec3f const& Transform::GetLocalFront() const {
	return localAxes.z;
}

vec3f const& Transform::GetLocalRight() const {
	return localAxes.x;
}

vec3f const& Transform::GetWorldTranslation() const {
	return worldTranslation;
}

vec3f const& Transform::GetWorldUp() const {
	return worldAxes.y;
}

vec3f const& Transform::GetWorldFront() const {
	return worldAxes.z;
}

vec3f const& Transform::GetWorldRight() const {
	return worldAxes.x;
}

vec3f const& Transform::GetWorldRotation() const {
	return worldRotation;
}

mat4f const& Transform::GetLocalTransform() const {
	return localTransform;
}

mat4f const& Transform::GetWorldTransform() const {
	return worldTransform;
}

mat4f Transform::GetLocalLookAt() const {
	const vec3f target = translation + GetLocalFront();
	const vec3f up = GetLocalUp();

	return Math::LookAt(translation, target, up);
}

void Transform::SetParent(Transform * const transform) {
	if (parent) {
		Helpers::Remove(parent->children, this);
		translation = worldTranslation;
		rotation = worldRotation;
	}

	parent = transform;

	if (parent) {
		translation = translation - parent->worldTranslation;
		rotation = rotation - parent->worldRotation;
		Helpers::Insert(parent->children, this);
	}
}

Transform * const Transform::GetParent() const {
	return parent;
}
