#ifndef TEST_H
#define TEST_H

#include "Scene.h"
#include "Camera.h"
#include "Text.h"
#include "Transform.h"

#include <Events/Event.h>

class Test : public Scene { 

	vec2f mousePosition;
	bool mouseDown;
	unsigned current;

	Camera* camera;
	Text* debugText;

public:

	void Awake() override;

	void Update(const float& dt) override;

	void KeyHandler(Events::Event* event);
	void CursorHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

private:

	unsigned Create(const vec2f& position, const vec4f& color);

	unsigned CreateFishFood(const vec2f& position);
	unsigned CreateFish(const vec2f& position);

	void CollisionEnter(unsigned target);
	void CollisionExit(unsigned target);

};

#endif
