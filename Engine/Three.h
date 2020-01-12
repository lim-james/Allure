#ifndef THREE_H
#define THREE_H

#include "Scene.h"

class Three : public Scene {

public:

	void Awake() override;

	void InputHandler(Events::Event* event);

};

#endif
