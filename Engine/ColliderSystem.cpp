#include "ColliderSystem.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>

void ColliderSystem::Initialize() {
	checks[SPHERE_COLLIDER][SPHERE_COLLIDER].Bind(&ColliderSystem::SphereSphere, this);

	EventsManager::Get()->Subscribe("BOX_COLLIDER_ACTIVE", &ColliderSystem::BoxActiveHandler, this);
	EventsManager::Get()->Subscribe("SPHERE_COLLIDER_ACTIVE", &ColliderSystem::SphereActiveHandler, this);
	EventsManager::Get()->Subscribe("RAY_CAST", &ColliderSystem::RayCastHandler, this);
}

void ColliderSystem::Update(float const & dt) {}

void ColliderSystem::FixedUpdate(float const & dt) {
	for (unsigned i = 0; i < COLLIDER_TYPE_COUNT; ++i) {
		auto& group = colliders[i];
		
		auto& allChecks = checks[i];
		if (allChecks.empty()) continue;

		for (auto it1 = group.begin(); it1 != group.end(); ++it1) {
			Collider* const c1 = *it1;
			const unsigned e1 = c1->entity;
			const unsigned l1 = entities->GetLayer(e1);

			if (allChecks.find(i) != allChecks.end()) {
				auto& detection = allChecks[i];

				for (auto it2 = it1 + 1; it2 != group.end(); ++it2) {
					Collider* const c2 = *it2;
					const unsigned e2 = c2->entity;
					const unsigned l2 = entities->GetLayer(e2);

					if (c1->ignoreMask == l2 || c2->ignoreMask == l1) continue;
					detection(c1, c2);
				}
			}
		}
	}
}

void ColliderSystem::BoxActiveHandler(Events::Event * event) {
	Collider* const c = static_cast<Events::AnyType<BoxCollider*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(colliders[BOX_COLLIDER], c);
	} else {
		if (Helpers::Remove(colliders[BOX_COLLIDER], c)) {
			for (auto& pair : history[c]) {
				pair.second = false;
			}
		}
	}
}

void ColliderSystem::SphereActiveHandler(Events::Event * event) {
	Collider* const c = static_cast<Events::AnyType<SphereCollider*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(colliders[SPHERE_COLLIDER], c);
	} else {
		Helpers::Remove(colliders[SPHERE_COLLIDER], c);
	}
}

void ColliderSystem::RayCastHandler(Events::Event * event) {
	const auto rayCast = static_cast<Events::RayCast*>(event);

	float distance = -1.0f;
	rayCast->result->distance = -1.0f;
	rayCast->result->target = nullptr;
	
	for (Collider* const c : colliders[BOX_COLLIDER]) {
		if (rayCast->collisionMask != entities->GetLayer(c->entity)) continue;

		BoxCollider* const box = static_cast<BoxCollider*>(c);
		CollisionData result = RayBox(rayCast->ray, box);
		if (result.distance >= 0) {
			if (distance < 0 || result.distance < distance) {
				rayCast->result->distance = result.distance;
				rayCast->result->position = result.position;
				rayCast->result->normal = result.normal;
				rayCast->result->target = box;
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

void ColliderSystem::PerformAction(unsigned const & action, Collider * const a, Collider * const b) {	
	a->handlers[action](b->entity);
	b->handlers[action](a->entity);
}

void ColliderSystem::SphereSphere(Collider * const a, Collider * const b) {
	SphereCollider* const c1 = static_cast<SphereCollider*>(a);
	SphereCollider* const c2 = static_cast<SphereCollider*>(b);

	Transform* const t1 = entities->GetComponent<Transform>(c1->entity);
	Transform* const t2 = entities->GetComponent<Transform>(c2->entity);

	const float distSq = Math::LengthSquared(t2->GetWorldTranslation() - t1->GetWorldTranslation());
	const float minDist = c1->radius + c2->radius;

	bool& wasInside = history[a][b];
	
	if (distSq < minDist * minDist) {
		// inside
		if (wasInside) {
			PerformAction(COLLISION_STAY, a, b);
		} else {
			wasInside = true;
			PerformAction(COLLISION_ENTER, a, b);
		}
	} else {
		// outside
		if (wasInside) {
			wasInside = false;
			PerformAction(COLLISION_EXIT, a, b);
		}
	}
}
