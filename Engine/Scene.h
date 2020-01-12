#ifndef SCENE_H
#define SCENE_H

#include "EntityManager.h"
//#include "ComponentsManager.h"
#include "SystemManager.h"

#include <Events/Event.h>

namespace Events {
	typedef AnyType<std::string> PresentScene;
}


class Scene {

protected:

	EntityManager* entities;
	//ComponentsManager* components;
	SystemManager* systems;

public:

	// Initialization of local variables
	Scene();
	// Destruction
	virtual ~Scene();

	// subscribe to events, subscribe ECS
	virtual void Awake();
	// subscribe to events
	virtual void Reset();
	// creation of game objects
	virtual void Start();

	// updates every fixed timestep
	virtual void FixedUpdate(const float& dt);
	// updates every frame
	virtual void Update(const float& dt);

	// unsubscribe events
	virtual void Stop();
	virtual void Destroy();

	virtual void PrepareForSegue(Scene* destination);

};

#endif
