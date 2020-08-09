#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"

#include <Math/Vectors.h>
#include <Handler/Handler.h>

#define MOUSE_CLICK	0
#define MOUSE_DOWN	1
#define MOUSE_OUT	2
#define MOUSE_OVER	3
#define MOUSE_UP	4
#define BUTTON_ACTION_COUNT 5

struct Button : Component {

	using base_type = Button;

	vec2f offset;
	vec2f scale;

	bool isEnabled, blockRaycast;
	Handler<void, unsigned> handlers[BUTTON_ACTION_COUNT];

	Button();

	void Initialize() override;
	Component* Clone() const override;
	void SetActive(bool const& state) override;

};

#endif
