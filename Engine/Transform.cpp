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

void Transform::SetActive(const bool & _active) {
	Component::SetActive(_active);
	Events::EventsManager::GetInstance()->Trigger("TRANSFORM_ACTIVE", new Events::AnyType<Transform*>(this));
}

Component * Transform::Clone() const {
	return new Transform(*this);
}

void Transform::Unpack() {
	//translation = data.Get<vec3f>("translation");
	//rotation = data.Get<vec3f>("rotation");
	//scale = data.Get<vec3f>("scale");
}

void Transform::Pack() {
	//data = vec4f("translation", translation);
	//data = vec4f("rotation", rotation);
	//data = vec4f("scale", scale);
}

void Transform::UpdateAxes() {
	const float yawRad = Math::Rad(rotation.y);
	const float pitchRad = Math::Rad(-rotation.x);
	const float rollRad = Math::Rad(rotation.z);

	localAxes.z.z = -cos(yawRad) * cos(pitchRad);
	localAxes.z.y = sin(pitchRad);
	localAxes.z.x = sin(yawRad) * cos(pitchRad);

	const vec3f worldUp(sin(rollRad), cos(rollRad), 0.f);

	localAxes.x = Math::Normalized(Math::Cross(localAxes.z, worldUp));
	localAxes.y = Math::Normalized(Math::Cross(localAxes.x, localAxes.z));

	if (parent) {
		const float yawRad = Math::Rad(worldRotation.y);
		const float pitchRad = Math::Rad(-worldRotation.x);
		const float rollRad = Math::Rad(worldRotation.z);

		worldAxes.z.z = -cos(yawRad) * cos(pitchRad);
		worldAxes.z.y = sin(pitchRad);
		worldAxes.z.x = sin(yawRad) * cos(pitchRad);

		const vec3f worldUp(sin(rollRad), cos(rollRad), 0.f);

		worldAxes.x = Math::Normalized(Math::Cross(worldAxes.z, worldUp));
		worldAxes.y = Math::Normalized(Math::Cross(worldAxes.x, worldAxes.z));
	}

	for (auto& child : children) {
		child->UpdateAxes();
	}
}

const vec3f & Transform::GetLocalUp() const {
	return localAxes.y;
}

const vec3f & Transform::GetLocalFront() const {
	return localAxes.z;
}

const vec3f & Transform::GetLocalRight() const {
	return localAxes.x;
}

const vec3f& Transform::GetWorldTranslation() const {
	return worldTranslation;
}

const vec3f & Transform::GetWorldUp() const {
	return worldAxes.y;
}

const vec3f & Transform::GetWorldFront() const {
	return worldAxes.z;
}

const vec3f & Transform::GetWorldRight() const {
	return worldAxes.x;
}

const vec3f & Transform::GetWorldRotation() const {
	return worldRotation;
}

const mat4f& Transform::GetLocalTransform() const {
	return localTransform;
}

const mat4f& Transform::GetWorldTransform() const {
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
