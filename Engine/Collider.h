#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

#include <Handler/Handler.h>
#include <vector>

#define COLLIDER_ENTER			0
#define COLLIDER_STAY			1
#define COLLIDER_EXIT			2
#define COLLIDER_ACTION_COUNT	3

struct Collider : Component {

	std::string tag;
	std::vector<std::string> ignore;

	Handler<void, unsigned> handlers[COLLIDER_ACTION_COUNT];

	Collider();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

};

#endif
