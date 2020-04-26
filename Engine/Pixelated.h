#ifndef PIXELATED_H
#define PIXELATED_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct Pixelated : PostProcess {

	float size;

	Pixelated();
	~Pixelated();

	void Initialize() override;
	Component* Clone() const override;

	void PreRender() override;
	void PostRender() override;
	void Render() override;

private:

	Shader* shader;
	Framebuffer* fbo;

	void ResizeHandler(Events::Event* event);

};

#endif
