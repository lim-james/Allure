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

	virtual void PreRender();
	virtual void RenderDepth(RendererData const& data) = 0;
	virtual void RenderOpaque(RendererData const& data) = 0;
	virtual void RenderTransparent(RendererData const& data) = 0;
	virtual void PostRender();

};

#endif
