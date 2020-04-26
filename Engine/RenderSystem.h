#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"
#include "Light.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "PostProcessStack.h"
#include "Shader.h"

#include <Events/Event.h>

#include <vector>
#include <map>

class RenderSystem : public System {

	std::vector<Light*> lights;

	std::vector<Camera*> cameras;
	std::vector<Camera*> fbCameras;

	std::vector<Renderer*> renderers;
	PostProcessStack* postProccessing;

	vec2f windowSize;

public:

	~RenderSystem() override;

	void Initialize() override;
	void Update(float const& dt) override;

	void Start() override;
	void Stop() override;

private:

	void LightActiveHandler(Events::Event* event);

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);
	void CameraFramebufferHandler(Events::Event* event);

	void ResizeHandler(Events::Event* event);

	void FBRender();
	void Render();
	void Render(RendererData const& data);
	
};

#endif
