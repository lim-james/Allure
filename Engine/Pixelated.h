#ifndef PIXELATED_H
#define PIXELATED_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct Pixelated : PostProcess {

	using base_type = PostProcess;

	float size;

	Pixelated();
	~Pixelated();

	void Initialize() override;
	Component* Clone() const override;

	void PreRender() override;
	void PostRender() override;
	void Render() override;

	void Render(unsigned const& tex);

private:

	Shader* shader;
	Framebuffer* fbo;

	void ResolutionHandler(Events::Event* event);

};

#endif
