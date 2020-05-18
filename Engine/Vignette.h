#ifndef VIGNETTE_H
#define VIGNETTE_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct Vignette : PostProcess {

	float multiplier;
	vec3f tint;

	Vignette();
	~Vignette();

	void Initialize() override;
	Component* Clone() const override;

	void PreRender() override;
	void PostRender() override;
	void Render() override;

private:

	Shader* shader;
	Framebuffer* fbo;

	void ResolutionHandler(Events::Event* event);

};

#endif
