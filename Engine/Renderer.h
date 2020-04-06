#ifndef RENDERER_H
#define RENDERER_H

#include "EntityManager.h"	
#include "Camera.h"

class Renderer {

protected:

	EntityManager* entities;

public:

	virtual ~Renderer();

	virtual void Initialize(EntityManager* const manager);
	virtual void Render(Camera* const camera) = 0;

};

#endif
