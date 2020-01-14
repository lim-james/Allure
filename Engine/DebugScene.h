#ifndef DEBUG_SCENE_H
#define DEBUG_SCENE_H

#include "Scene.h"

#include "Camera.h"

#include <Events/Event.h>

class DebugScene : public Scene {

	Camera* camera;

	unsigned square;

public:

	void Awake() override;

private:

	void KeyHandler(Events::Event * event);

	void OnMouseOver(unsigned target);
	void OnMouseOut(unsigned target);

};

#endif
