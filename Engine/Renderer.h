#ifndef RENDERER_H
#define RENDERER_H

#include "EntityManager.h"	
#include "Light.h"
#include "Camera.h"

struct RendererData {
	Camera* camera;

	mat4f projection;
	mat4f view;

	std::vector<Light*>* lights;
};

class Renderer {

protected:

	EntityManager* entities;

public:

	virtual ~Renderer();

	virtual void Initialize(EntityManager* const manager);
	virtual void Render(RendererData const& data) = 0;

};

#endif
