#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"

#include <Encoding/APO.h>
#include <Math/Vectors.h>
#include <Math/Mat4.hpp>
#include <Containers/Quad.hpp>

#include <vector>

struct Transform : Component, APO {

	vec3f translation;
	vec3f rotation;
	vec3f scale;

	std::vector<Quad<Transform*>*> quads;

	Transform();

	// component abstract methods

	void Initialize() override;
	void SetActive(const bool& _active) override;

	// APO methods

	void Unpack() override;
	void Pack() override;

	// transform methods

	void UpdateAxes();

	const vec3f& GetLocalUp() const;
	const vec3f& GetLocalFront() const;
	const vec3f& GetLocalRight() const;

	const vec3f& GetWorldTranslation() const;

	const vec3f& GetWorldUp() const;
	const vec3f& GetWorldFront() const;
	const vec3f& GetWorldRight() const;

	const vec3f& GetWorldRotation() const;

	const mat4f& GetLocalTransform() const;
	const mat4f& GetWorldTransform() const;

	mat4f GetLocalLookAt() const;

	void SetParent(Transform * const transform);
	Transform * const GetParent() const;

private:

	Transform* parent;
	std::vector<Transform*> children;

	vec3f worldRotation;
	vec3f worldTranslation;

	Math::vec<3, vec3f> localAxes;
	Math::vec<3, vec3f> worldAxes;
		
	mat4f localTransform;
	mat4f worldTransform;

	friend class TransformSystem;

};

#endif
