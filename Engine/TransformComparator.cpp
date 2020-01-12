#include "TransformComparator.h"

void TransformComparator::Partition(Quad<Transform*>* parent) {
	for (auto& item : parent->list) {
		vec2f min, max;
		min = item->GetWorldTranslation() - item->scale * 0.5f;
		max = item->GetWorldTranslation() + item->scale * 0.5f;

		if (min.x <= parent->position.x) {
			if (min.y <= parent->position.y) {
				Add(item, parent->children[BOTTOM_LEFT]);
			} 

			if (max.y >= parent->position.y) {
				Add(item, parent->children[TOP_LEFT]);
			}
		}

		if (max.x >= parent->position.x) {
			if (min.y <= parent->position.y) {
				Add(item, parent->children[BOTTOM_RIGHT]);
			} 

			if (max.y >= parent->position.y) {
				Add(item, parent->children[TOP_RIGHT]);
			}
		}
	}
}

void TransformComparator::Add(Transform* const item, Quad<Transform*>* const quad) const {
	quad->list.push_back(item);

	const unsigned size = item->quads.size();
	unsigned i = 0;
	for (; i < size; ++i) {
		if (item->quads[i] == quad->parent) {
			item->quads[i] = quad;
			return;
		}
	}

	if (i == size) {
		item->quads.push_back(quad);
	}
}
