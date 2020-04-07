#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Math/Mat4.hpp>
#include <Containers/Quad.hpp>

#include <vector>

struct Transform : Component {

	vec3f translation;
	vec3f rotation;
	vec3f scale;

	std::vector<Quad<Transform*>*> quads;

	Transform();

	// component abstract methods

	void Initialize() override;
	void SetActive(bool const& state) override;
	Component* Clone() const override;

	// AEObject methods

	void Unpack(AENotation const& data) override;
	void Pack(AENotation& data) override;

	// transform methods

	void UpdateAxes();

	vec3f const& GetLocalUp() const;
	vec3f const& GetLocalFront() const;
	vec3f const& GetLocalRight() const;

	vec3f const& GetWorldTranslation() const;

	vec3f const& GetWorldUp() const;
	vec3f const& GetWorldFront() const;
	vec3f const& GetWorldRight() const;

	vec3f const& GetWorldRotation() const;

	mat4f const& GetLocalTransform() const;
	mat4f const& GetWorldTransform() const;

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
