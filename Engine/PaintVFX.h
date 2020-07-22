#ifndef PAINT_VFX_H
#define PAINT_VFX_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct PaintVFX : PostProcess {

	using base_type = PostProcess;

	float unit;
	float radius;

	PaintVFX();
	~PaintVFX();

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
