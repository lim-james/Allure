#ifndef SCENE_H
#define SCENE_H

#include "EntityManager.h"
#include "SystemManager.h"

#include <Events/Event.h>

class Scene {

protected:

	static int count;
	std::string name;

	EntityManager* entities;
	SystemManager* systems;
	
	unsigned mainCamera;

public:

	// Initialization of local variables
	Scene();
	// Destruction
	virtual ~Scene();

	// subscribe to events, subscribe ECS
	virtual void Awake();
	// creation of game objects
	virtual void Create();
	// subscribe to events
	virtual void Enter();

	// updates every fixed timestep
	virtual void FixedUpdate(const float& dt);
	// updates every frame
	virtual void Update(const float& dt);

	// unsubscribe events
	virtual void Exit();
	virtual void Destroy();

	virtual void PrepareForSegue(Scene* destination);

};

namespace Events {
	typedef AnyType<Scene*> PresentScene;
}

#endif
