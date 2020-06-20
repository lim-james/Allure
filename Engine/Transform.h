#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Math/Mat4.hpp>
#include <Containers/Quad.hpp>

#include <vector>

struct Transform : Component {

	std::vector<Quad<Transform*>*> quads;

	Transform();

	// component abstract methods

	void Initialize() override;
	void SetActive(bool const& state) override;
	void SetDynamic(bool const& state) override;
	Component* Clone() const override;

	// AEObject methods

	void Unpack(AENotation const& data) override;
	void Pack(AENotation& data) override;

	// transform methods

	void SetParent(Transform * const transform);
	Transform * const GetParent() const;

	std::vector<Transform*> const& GetChildren() const;

	// local

	vec3f const& GetScale() const;
	vec3f const& GetLocalRotation() const;
	vec3f const& GetLocalTranslation() const;

	void SetScale(vec3f const& scale);
	void SetLocalRotation(vec3f const& rotation);
	void SetLocalTranslation(vec3f const& translation);

	// world

	vec3f const& GetWorldRotation();
	vec3f const& GetWorldTranslation();

	// local axes
	
	vec3f const& GetLocalUp();
	vec3f const& GetLocalFront();
	vec3f const& GetLocalRight();

	// world axes

	vec3f const& GetWorldUp();
	vec3f const& GetWorldFront();
	vec3f const& GetWorldRight();

	mat4f const& GetLocalTransform();
	mat4f const& GetWorldTransform();

	// look at

	mat4f GetLocalLookAt();
	mat4f GetWorldLookAt();

private:

	bool shouldUpdateLocal, shouldUpdateWorld;
	bool shouldUpdateAxes;

	Transform* parent;
	std::vector<Transform*> children;

	vec3f scale;
	vec3f localRotation;
	vec3f localTranslation;

	vec3f worldRotation;
	vec3f worldTranslation;

	Math::vec<3, vec3f> localAxes;
	Math::vec<3, vec3f> worldAxes;
		
	mat4f localTransform;
	mat4f worldTransform;

	void LocalChanged();
	void WorldChanged();
	void AxesChanged();

	void UpdateLocal();
	void UpdateWorld();
	void UpdateAxes();

	friend class TransformSystem;

};

#endif
