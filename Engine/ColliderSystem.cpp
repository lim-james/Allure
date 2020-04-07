#include "ColliderSystem.h"

#include "Transform.h"

#include <Events/EventsManager.h>
#include <Helpers/VectorHelpers.h>
#include <MACROS.h>

ColliderSystem::~ColliderSystem() {
	components.clear();
}

void ColliderSystem::Initialize() {
	Events::EventsManager::GetInstance()->Subscribe("COLLIDER_ACTIVE", &ColliderSystem::ActiveHandler, this);
}

void ColliderSystem::Update(float const& dt) {
	for (auto& c1 : components) {
		auto const& p1 = c1->entity;
		auto t1 = entities->GetComponent<Transform>(p1);

		const vec2f position1 = t1->GetWorldTranslation();
		vec2f halfSize1 = t1->scale * 0.5f;
		halfSize1.y = -halfSize1.y;
		const vec2f min1 = position1 - halfSize1;
		const vec2f max1 = position1 + halfSize1;

		for (auto& quad : t1->quads) {
			for (auto& t2 : quad->list) {
				if (t1 == t2) continue;

				auto const& p2 = t2->entity;
				auto c2 = entities->GetComponent<Collider>(p2);
				if (!c2) continue;

				bool ignore = false;
				for (auto& tag : c1->ignore) {
					if (tag == c2->tag) {
						ignore = true;
						break;
					}
				}
				if (ignore) continue;

				const vec2f position2 = t2->GetWorldTranslation();
				vec2f halfSize2 = t2->scale * 0.5f;
				halfSize2.y = -halfSize2.y;
				const vec2f min2 = position2 - halfSize2;
				const vec2f max2 = position2 + halfSize2;

				if (!(min1.x > max2.x // right
					|| max1.x < min2.x // left
					|| min1.y < max2.y // top
					|| max1.y > min2.y)) { // down
					if (history[p1][p2] && history[p2][p1]) {
						PerformAction(COLLIDER_STAY, c1, c2);
					} else {
						PerformAction(COLLIDER_ENTER, c1, c2);
						history[p1][p2] = history[p2][p1] = true;
					}
				} else {
					if (history[p1][p2] || history[p2][p1]) {
						PerformAction(COLLIDER_EXIT, c1, c2);
						history[p1][p2] = history[p2][p1] = false;
					}
				}
			}
		}
	}
}

// TODO: Consider if checks can be avoided
void ColliderSystem::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<Collider*>*>(event)->data;
	const auto position = vfind(components, c);

	if (c->IsActive()) {
		Helpers::Insert(components, c);
	} else {
		if (Helpers::Remove(components, c)) {
			for (auto& pair : history[c->entity]) {
				pair.second = false;
			}
		}
	}
}

void ColliderSystem::PerformAction(unsigned const& index, Collider * const c1, Collider * const c2) {
	c1->handlers[index](c2->entity);
	c2->handlers[index](c1->entity);
}