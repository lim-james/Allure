#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "BaseScript.h"

#include <Events/Event.h>
#include <vector>

class ScriptSystem {

	std::vector<BaseScript*> scripts;

public:

	ScriptSystem();
	~ScriptSystem();

	void Awake();
	void Start();
	void FixedUpdate();
	void Update();
	void Stop();
	void Destroy();

private:

	void ActiveHandler(Events::Event* event);

};

#endif
