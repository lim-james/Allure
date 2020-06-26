#include "ColliderSystem.h"

#include "Transform.h"

#include <Helpers/VectorHelpers.h>
#include <Events/EventsManager.h>
#include <Math/Mat4Transform.hpp>

void ColliderSystem::Initialize() {
	checks[SPHERE_COLLIDER][SPHERE_COLLIDER].Bind(&ColliderSystem::SphereSphere, this);
	checks[SPHERE_COLLIDER][LINE_COLLIDER].Bind(&ColliderSystem::SphereLine, this);

	EventsManager::Get()->Subscribe("BOX_COLLIDER_ACTIVE", &ColliderSystem::BoxActiveHandler, this);
	EventsManager::Get()->Subscribe("SPHERE_COLLIDER_ACTIVE", &ColliderSystem::SphereActiveHandler, this);
	EventsManager::Get()->Subscribe("LINE_COLLIDER_ACTIVE", &ColliderSystem::LineActiveHandler, this);
	EventsManager::Get()->Subscribe("RAY_CAST", &ColliderSystem::RayCastHandler, this);
}

void ColliderSystem::Update(float const & dt) {}

void ColliderSystem::FixedUpdate(float const & dt) {
	for (unsigned i = 0; i < COLLIDER_TYPE_COUNT; ++i) {
		auto& group1 = colliders[i];
		
		auto& allChecks = checks[i];
		if (allChecks.empty()) continue;

		for (auto it1 = group1.begin(); it1 != group1.end(); ++it1) {
			Collider* const c1 = *it1;
			const unsigned e1 = c1->entity;
			const unsigned l1 = entities->GetLayer(e1);

			if (allChecks.find(i) != allChecks.end()) {
				auto& detection = allChecks[i];

				for (auto it2 = it1 + 1; it2 != group1.end(); ++it2) {
					Collider* const c2 = *it2;
					const unsigned e2 = c2->entity;
					const unsigned l2 = entities->GetLayer(e2);

					if (c1->ignoreMask == l2 || c2->ignoreMask == l1) continue;
					detection.Invoke(c1, c2);
				}
			}

			for (unsigned j = i + 1; j < COLLIDER_TYPE_COUNT; ++j) {
				auto& group2 = colliders[j];

				if (allChecks.find(j) != allChecks.end()) {
					auto& detection = allChecks[j];

					for (auto& c2 : group2) {
						const unsigned e2 = c2->entity;
						const unsigned l2 = entities->GetLayer(e2);

						if (c1->ignoreMask == l2 || c2->ignoreMask == l1) continue;
						detection.Invoke(c1, c2);
					}
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
		if (Helpers::Remove(colliders[SPHERE_COLLIDER], c)) {
			for (auto& pair : history[c]) {
				pair.second = false;
			}
		}
	}
}

void ColliderSystem::LineActiveHandler(Events::Event * event) {
	Collider* const c = static_cast<Events::AnyType<LineCollider*>*>(event)->data;

	if (c->IsActive()) {
		Helpers::Insert(colliders[LINE_COLLIDER], c);
	} else {
		if (Helpers::Remove(colliders[LINE_COLLIDER], c)) {
			for (auto& pair : history[c]) {
				pair.second = false;
			}
		}
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
	const vec3f boxScale = boxTransform->GetScale() * box->scale * 0.5f;
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

void ColliderSystem::HandleCollision(Collider * const a, Collider * const b, bool const & intersecting) {
	bool& wasIntersecting = history[a][b];
	
	if (intersecting) {
		// inside
		if (wasIntersecting) {
			PerformAction(COLLISION_STAY, a, b);
		} else {
			wasIntersecting = true;
			PerformAction(COLLISION_ENTER, a, b);
		}
	} else {
		// outside
		if (wasIntersecting) {
			wasIntersecting = false;
			PerformAction(COLLISION_EXIT, a, b);
		}
	}
}

void ColliderSystem::PerformAction(unsigned const & action, Collider * const a, Collider * const b) {
	a->handlers[action].Invoke(b->entity);
	b->handlers[action].Invoke(a->entity);
}

void ColliderSystem::SphereSphere(Collider * const a, Collider * const b) {
	SphereCollider* const c1 = static_cast<SphereCollider*>(a);
	SphereCollider* const c2 = static_cast<SphereCollider*>(b);

	Transform* const t1 = entities->GetComponent<Transform>(c1->entity);
	Transform* const t2 = entities->GetComponent<Transform>(c2->entity);

	const vec3f s1 = t1->GetScale() * c1->scale;
	const vec3f s2 = t2->GetScale() * c2->scale;

	const float r1 = max(s1.x, max(s1.y, s1.z)) * 0.5f;
	const float r2 = max(s2.x, max(s2.y, s2.z)) * 0.5f;

	const float distSq = Math::LengthSquared((t2->GetWorldTranslation() + c2->offset) - (t1->GetWorldTranslation() + c1->offset));
	const float minDist = r1 + r2;

	HandleCollision(a, b, distSq < minDist * minDist);
}

void ColliderSystem::SphereLine(Collider * const a, Collider * const b) {
	SphereCollider* const c1 = static_cast<SphereCollider*>(a);
	LineCollider* const c2 = static_cast<LineCollider*>(b);

	Transform* const t1 = entities->GetComponent<Transform>(c1->entity);
	Transform* const t2 = entities->GetComponent<Transform>(c2->entity);

	const vec3f s1 = t1->GetScale() * c1->scale;
	const vec3f s2 = t2->GetScale() * c2->scale;

	const vec3f p1 = t1->GetWorldTranslation() + c1->offset;
	const vec3f p2 = t2->GetWorldTranslation() + c2->offset;

	const float r = max(s1.x, max(s1.y, s1.z)) * 0.5f;

	vec3f half = c2->direction * s2 * (c2->length * 0.5f);
	Math::Rotate(half, t2->GetWorldRotation());
	const vec3f E = p2 + half;
	const vec3f L = p2 - half;

	const vec3f d = L - E;
	const vec3f f = E - p1;

	float A = Math::Dot(d, d) * 2.f;
	const float B = 2.f * Math::Dot(f, d);
	const float C = Math::Dot(f, f) - r * r;

	bool isIntersecting = false;

	float discriminant = B * B - 2.f * A * C;
	if (discriminant >= 0) {
		discriminant = sqrt(discriminant);
		const float t1 = (-B - discriminant) / A;
		const float t2 = (-B + discriminant) / A;

		isIntersecting = (t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1);
	}
	
	HandleCollision(a, b, isIntersecting);
}
