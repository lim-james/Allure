#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

#include <vector>
#include <functional>

#define COLLIDER_ENTER			0
#define COLLIDER_STAY			1
#define COLLIDER_EXIT			2
#define COLLIDER_ACTION_COUNT	3

struct Collider : Component {

	std::string tag;
	std::vector<std::string> ignore;

	Collider();

	void Initialize() override;
	void SetActive(const bool& state) override;

	template<typename Context>
	void BindHandler(const unsigned& action, void(Context::*callback)(unsigned), Context* context);
	template<typename Context>
	void BindHandler(const unsigned& action, void(Context::*callback)(unsigned) const, Context* context);

private:

	std::function<void(unsigned)> handlers[COLLIDER_ACTION_COUNT];

	friend class ColliderSystem;

};

template<typename Context>
void Collider::BindHandler(const unsigned& action, void(Context::*callback)(unsigned), Context* context) {
	handlers[action] = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void Collider::BindHandler(const unsigned& action, void(Context::*callback)(unsigned) const, Context* context) {
	handlers[action] = std::bind(callback, context, std::placeholders::_1);
}


#endif
