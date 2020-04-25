#include "ColliderSystem.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

void ColliderSystem::Initialize() {
	Events::EventsManager::GetInstance()->Subscribe("BOX_COLLIDER_ACTIVE", &ColliderSystem::ActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("RAY_CAST", &ColliderSystem::RayCastHandler, this);
}

void ColliderSystem::Update(float const & dt) {}

void ColliderSystem::ActiveHandler(Events::Event * event) {
	BoxCollider* const c = static_cast<Events::AnyType<BoxCollider*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(components, c);
	} else {
		Helpers::Remove(components, c);
	}
}

void ColliderSystem::RayCastHandler(Events::Event * event) {
	const auto rayCast = static_cast<Events::RayCast*>(event);

	float distance = -1.0f;
	rayCast->result->distance = -1.0f;
	rayCast->result->target = nullptr;
	
	for (BoxCollider* const collider : components) {
		if (rayCast->collisionMask != entities->GetLayer(collider->entity)) continue;

		CollisionData result = RayBox(rayCast->ray, collider);
		if (result.distance >= 0) {
			if (distance < 0 || result.distance < distance) {
				rayCast->result->distance = result.distance;
				rayCast->result->position = result.position;
				rayCast->result->normal = result.normal;
				rayCast->result->target = collider;
				distance = result.distance;
			}
		}
	}
}

CollisionData ColliderSystem::RayBox(Ray const & ray, BoxCollider * const box) {
	CollisionData result;
	result.distance = -1.0f;

	Transform* const boxTransform = entities->GetComponent<Transform>(box->entity);
	const vec3f boxTranslation = boxTransform->GetWorldTranslation() + box->offset;
	const vec3f boxScale = boxTransform->scale * box->scale * 0.5f;
	const vec3f min = boxTranslation - boxScale;
	const vec3f max = boxTranslation + boxScale;

	const vec3f normals[3] = {
		vec3f(1.f, 0.f, 0.f),
		vec3f(0.f, 1.f, 0.f),
		vec3f(0.f, 0.f, 1.f)
	};

	for (vec3f const& N : normals) {
		float dot = Math::Dot(N, ray.direction);
		if (dot == 0) continue;

		Plane plane;

		if (dot < 0) {
			plane.normal = N;
		} else {
			plane.normal = -N;
			dot = -dot;
		}

		plane.origin = boxTranslation + boxScale * plane.normal;
		const float t = Math::Dot(plane.origin - ray.origin, plane.normal) / dot;
		const vec3f position = ray.origin + ray.direction * t;

		if (position.x >= min.x && position.x <= max.x &&
			position.y >= min.y && position.y <= max.y &&
			position.z >= min.z && position.z <= max.z) {
			result.normal = plane.normal;
			result.position = position;
			result.distance = t;
			break;
		}
	}

	return result;
}
