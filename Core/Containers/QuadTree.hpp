#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "Quad.hpp"
#include "Comparator.h"

#include "../Helpers/VectorHelpers.h"
#include "../MACROS.h"

template<typename ContentType, typename ComparatorType>
class QuadTree {

	const unsigned MAX_ITEMS;
	const unsigned MAX_DEPTH;

	Quad<ContentType>* root;
	Comparator<ContentType>* comparator;

public:

	QuadTree(unsigned const& items, unsigned const& depth);
	
	void Sort();
	void Clear();

	bool Insert(ContentType const& item);
	bool Insert(ContentType const& item, bool const& ascending);
	bool Remove(ContentType const& item);

	Quad<ContentType> * const GetRoot() const;

	template<typename ContentType, typename ComparatorType>
	friend std::ostream& operator<<(std::ostream& os, QuadTree<ContentType, ComparatorType> const& tree);
	
private:

	void Sort(Quad<ContentType>* parent) const;
	void Split(Quad<ContentType>* parent) const;
	void Delete(Quad<ContentType>* parent) const;

};

template<typename ContentType, typename ComparatorType>
QuadTree<ContentType, ComparatorType>::QuadTree(unsigned const& items, unsigned const& depth) 
	: MAX_ITEMS(items)
	, MAX_DEPTH(depth) {
	// TODO: change hardcoded size
	root = new Quad<ContentType>(nullptr, vec2f(0.f), vec2f(10.f));
	comparator = new ComparatorType;
}

template<typename ContentType, typename ComparatorType>
void QuadTree<ContentType, ComparatorType>::Sort() {
	Sort(root);
}

template<typename ContentType, typename ComparatorType>
void QuadTree<ContentType, ComparatorType>::Clear() {
	if (root->children[0]) {
		for (auto& child : root->children) {
			Delete(child);
			child = nullptr;
		}
	}
}

template<typename ContentType, typename ComparatorType>
bool QuadTree<ContentType, ComparatorType>::Insert(ContentType const& item) {
	return Helpers::Insert(root->list, item);
}

template<typename ContentType, typename ComparatorType>
bool QuadTree<ContentType, ComparatorType>::Insert(ContentType const& item, bool const& ascending) {
	return Helpers::Insert(root->list, item, ascending);
}

template<typename ContentType, typename ComparatorType>
bool QuadTree<ContentType, ComparatorType>::Remove(ContentType const& item) {
	return Helpers::Remove(root->list, item);
}

template<typename ContentType, typename ComparatorType>
Quad<ContentType> * const QuadTree<ContentType, ComparatorType>::GetRoot() const {
	return root;
}

template<typename ContentType, typename ComparatorType>
void QuadTree<ContentType, ComparatorType>::Sort(Quad<ContentType>* parent) const {
	if (parent->depth == MAX_DEPTH)
		return;

	if (parent->list.size() > MAX_ITEMS) {
		Split(parent);
		comparator->Partition(parent);
		for (auto& child : parent->children)
			Sort(child);
	}
}

template<typename ContentType, typename ComparatorType>
void QuadTree<ContentType, ComparatorType>::Split(Quad<ContentType>* parent) const {
	if (parent->children[0] == nullptr) {
		const vec2f halfSize = parent->size * 0.5f;

		parent->children[TOP_LEFT] = new Quad<ContentType>(parent, parent->position, halfSize);
		parent->children[TOP_LEFT]->position.x -= halfSize.w;
		parent->children[TOP_LEFT]->position.y += halfSize.h;

		parent->children[TOP_RIGHT] = new Quad<ContentType>(parent, parent->position, halfSize);
		parent->children[TOP_RIGHT]->position.x += halfSize.w;
		parent->children[TOP_RIGHT]->position.y += halfSize.h;

		parent->children[BOTTOM_LEFT] = new Quad<ContentType>(parent, parent->position, halfSize);
		parent->children[BOTTOM_LEFT]->position.x -= halfSize.w;
		parent->children[BOTTOM_LEFT]->position.y -= halfSize.h;

		parent->children[BOTTOM_RIGHT] = new Quad<ContentType>(parent, parent->position, halfSize);
		parent->children[BOTTOM_RIGHT]->position.x += halfSize.w;
		parent->children[BOTTOM_RIGHT]->position.y -= halfSize.h;
	}
}

template<typename ContentType, typename ComparatorType>
void QuadTree<ContentType, ComparatorType>::Delete(Quad<ContentType>* parent) const {
	if (parent->children[0]) {
		for (auto& child : parent->children) {
			Delete(child);
			child = nullptr;
		}
	}

	delete parent;
}

template<typename ContentType, typename ComparatorType>
std::ostream & operator<<(std::ostream & os, QuadTree<ContentType, ComparatorType> const& tree) {
	os << *tree.root;
	return os;
}

#endif

