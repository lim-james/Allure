#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"
#include "Render.h"
#include "Camera.h"
#include "Text.h"
#include "Shader.h"
#include "Line.h"
#include "Framebuffer.h"

// renderers
#include "StandardRenderer.h"
#include "LineRenderer.h"
#include "TextRenderer.h"

#include <Events/Event.h>

#include <vector>
#include <map>

class RenderSystem : public System {

	std::vector<Camera*> cameras;
	std::vector<Renderer*> renderers;

	Shader* curveShader;
	Framebuffer* mainFBO;

	vec2f windowSize;

public:

	~RenderSystem() override;

	void Initialize() override;
	void Update(float const& dt) override;

private:

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);

	void ResizeHandler(Events::Event* event);

};

#endif
