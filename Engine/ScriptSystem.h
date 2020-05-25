#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "Script.h"

#include <Events/Event.h>
#include <vector>

class ScriptSystem {

	Time* time;

	EntityManager * const entities;
	bool started;

	std::vector<Script*> scripts;

public:

	ScriptSystem(EntityManager * const entities);
	~ScriptSystem();

	void Awake();
	void Start();
	void FixedUpdate(float const& dt);
	void Update(float const& dt);
	void Stop();
	void Destroy();

private:

	void ActiveHandler(Events::Event* event);

	void Setup(Script * const script);

};

#endif
