#include "Transform.h"

#include <Math/Mat4Transform.hpp>
#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

Transform::Transform()
	: shouldUpdateLocal(true)
	, shouldUpdateWorld(true)
	, shouldUpdateAxes(true)
	, parent(nullptr)
	, scale(1.f)
	, localRotation(0.f)
	, localTranslation(0.f)
	, worldRotation(0.f)
	, worldTranslation(0.f) {
	UpdateLocal();
	UpdateWorld();
	UpdateAxes();
}

void Transform::Initialize() {
	shouldUpdateLocal = true;
	shouldUpdateWorld = true;
	shouldUpdateAxes = true;

	parent = nullptr;
	for (int i = static_cast<int>(children.size()) - 1; i >= 0; --i)
		children[i]->SetParent(nullptr);
	children.clear();

	scale = vec3f(1.f);
	localRotation = vec3f(0.f);
	localTranslation = vec3f(0.f);

	worldRotation = vec3f(0.f);
	worldTranslation = vec3f(0.f);

	UpdateLocal();
	UpdateWorld();
	UpdateAxes();
}

void Transform::SetActive(bool const& state) {
	Component::SetActive(state);
	EventsManager::Get()->Trigger("TRANSFORM_ACTIVE", new Events::AnyType<Transform*>(this));
}

void Transform::SetDynamic(bool const & state) {
	Component::SetDynamic(state);
	EventsManager::Get()->Trigger("TRANSFORM_DYNAMIC", new Events::AnyType<Transform*>(this));
}

Component * Transform::Clone() const {
	return new Transform(*this);
}

void Transform::Unpack(AENotation const& data) {
	Component::Unpack(data);
	scale = data.Get<vec3f>("scale");
	localRotation = data.Get<vec3f>("local_rotation");
	localTranslation = data.Get<vec3f>("local_translation");
}

void Transform::Pack(AENotation& data) {
	data.Set("type", "Transform");
	Component::Pack(data);
	scale = data.Get<vec3f>("scale");
	localRotation = data.Get<vec3f>("local_rotation");
	localTranslation = data.Get<vec3f>("local_translation");
}

void Transform::SetParent(Transform * const transform) {
	if (parent) {
		Helpers::Remove(parent->children, this);
	}

	parent = transform;

	if (parent) {
		Helpers::Insert(parent->children, this);
	}

	LocalChanged();
}

Transform * const Transform::GetParent() const {
	return parent;
}

std::vector<Transform*> const & Transform::GetChildren() const {
	return children;
}

vec3f const& Transform::GetScale() const {
	return scale;
}

vec3f const& Transform::GetLocalRotation() const {
	return localRotation;
}

vec3f const& Transform::GetLocalTranslation() const {
	return localTranslation;
}

void Transform::SetScale(vec3f const & scale) {
	this->scale = scale;
}

void Transform::SetLocalRotation(vec3f const & rotation) {
	localRotation = rotation;
	LocalChanged();
	AxesChanged();
}

void Transform::SetLocalTranslation(vec3f const & translation) {
	localTranslation = translation;
	LocalChanged();
}

vec3f const& Transform::GetWorldRotation() {
	if (shouldUpdateWorld) UpdateWorld();
	return worldRotation;
}

vec3f const& Transform::GetWorldTranslation() {
	if (shouldUpdateWorld) UpdateWorld();
	return worldTranslation;
}

vec3f const& Transform::GetLocalUp() {
	if (shouldUpdateAxes) UpdateAxes();
	return localAxes.y;
}

vec3f const& Transform::GetLocalFront() {
	if (shouldUpdateAxes) UpdateAxes();
	return localAxes.z;
}

vec3f const& Transform::GetLocalRight() {
	if (shouldUpdateAxes) UpdateAxes();
	return localAxes.x;
}

vec3f const& Transform::GetWorldUp() {
	if (shouldUpdateAxes) UpdateAxes();
	return worldAxes.y;
}

vec3f const& Transform::GetWorldFront() {
	if (shouldUpdateAxes) UpdateAxes();
	return worldAxes.z;
}

vec3f const& Transform::GetWorldRight() {
	if (shouldUpdateAxes) UpdateAxes();
	return worldAxes.x;
}

mat4f const& Transform::GetLocalTransform() {
	if (shouldUpdateLocal) UpdateLocal();
	return localTransform;
}

mat4f const& Transform::GetWorldTransform() {
	if (shouldUpdateWorld) UpdateWorld();
	return worldTransform;
}

mat4f Transform::GetLocalLookAt() {
	const vec3f target = localTranslation + GetLocalFront();
	const vec3f up = GetLocalUp();

	return Math::LookAt(localTranslation, target, up);
}

mat4f Transform::GetWorldLookAt() {
	const vec3f target = GetWorldTranslation() + GetWorldFront();
	const vec3f up = GetWorldUp();

	return Math::LookAt(worldTranslation, target, up);
}

void Transform::LocalChanged() {
	shouldUpdateLocal = shouldUpdateWorld = true;
	for (Transform* const child : children)
		child->WorldChanged();
}

void Transform::WorldChanged() {
	shouldUpdateWorld = true;
	for (Transform* const child : children)
		child->WorldChanged();
}

void Transform::AxesChanged() {
	shouldUpdateAxes = true;
	for (Transform* const child : children)
		child->AxesChanged();
} 

void Transform::UpdateLocal() {
	Math::SetToTransform(
		localTransform,
		localTranslation,
		localRotation,
		scale
	);
	shouldUpdateLocal = false;
}

void Transform::UpdateWorld() {
	if (parent) {
		worldRotation = localRotation + parent->worldRotation;
		worldTranslation = parent->GetWorldTransform() * localTranslation;

		Math::SetToTransform(
			worldTransform,
			worldTranslation,
			worldRotation,
			scale
		);
	} else {
		worldRotation = localRotation;
		worldTranslation = localTranslation;
		worldTransform = GetLocalTransform();
	}

	shouldUpdateWorld = false;
}

void Transform::UpdateAxes() {
	float yawRad = localRotation.y * Math::toRad;
	float pitchRad = -localRotation.x * Math::toRad;
	float rollRad = localRotation.z * Math::toRad;

	localAxes.z.z = -cos(yawRad) * cos(pitchRad);
	localAxes.z.y = sin(pitchRad);
	localAxes.z.x = sin(yawRad) * cos(pitchRad);

	const vec3f worldUp(sin(rollRad), cos(rollRad), 0.f);

	localAxes.x = Math::Normalized(Math::Cross(localAxes.z, worldUp));
	localAxes.y = Math::Normalized(Math::Cross(localAxes.x, localAxes.z));

	if (parent) {
		const vec3f rotation = GetWorldRotation() * Math::toRad;

		yawRad = rotation.y;
		pitchRad = -rotation.x;
		rollRad = rotation.z;

		worldAxes.z.z = -cos(yawRad) * cos(pitchRad);
		worldAxes.z.y = sin(pitchRad);
		worldAxes.z.x = sin(yawRad) * cos(pitchRad);

		const vec3f worldUp(sin(rollRad), cos(rollRad), 0.f);

		worldAxes.x = Math::Normalized(Math::Cross(worldAxes.z, worldUp));
		worldAxes.y = Math::Normalized(Math::Cross(worldAxes.x, worldAxes.z));
	} else {
		worldAxes = localAxes;
	}

	shouldUpdateAxes = false;
}
