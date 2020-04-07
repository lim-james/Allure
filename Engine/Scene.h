#ifndef SCENE_H
#define SCENE_H

#include "EntityManager.h"
#include "SystemManager.h"
#include "ScriptSystem.h"

#include <Events/Event.h>

class Scene {

protected:

	EntityManager* entities;
	SystemManager* systems;
	ScriptSystem* scripts;

	std::string filepath;
	
	unsigned mainCamera;

public:

	// Initialization of local variables
	Scene();
	Scene(std::string const& filepath);
	// Destruction
	virtual ~Scene();

	// subscribe to events, subscribe ECS
	virtual void Awake();
	// creation of game objects
	virtual void Create();
	// subscribe to events
	virtual void Enter();

	// updates every fixed timestep
	virtual void FixedUpdate(float const& dt);
	// updates every frame
	virtual void Update(float const& dt);

	// unsubscribe events
	virtual void Exit();
	virtual void Destroy();

	virtual void PrepareForSegue(Scene* destination);

protected:

	void Load(std::string const& filepath);

};

namespace Events {
	typedef AnyType<Scene*> PresentScene;
}

#endif
