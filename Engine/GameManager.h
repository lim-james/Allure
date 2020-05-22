#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Script.h"

struct GameManager : Script {

private:

	void Awake() override;

	void KeyHanlder(Events::Event* event);

};

#endif
