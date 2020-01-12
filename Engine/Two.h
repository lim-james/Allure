#ifndef TWO_H
#define TWO_H

#include "Scene.h"

class Two : public Scene {

public:

	void Awake() override;

	void InputHandler(Events::Event* event);

};

#endif
