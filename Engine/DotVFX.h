#ifndef DOT_VFX_H
#define DOT_VFX_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct DotVFX : PostProcess {

	using base_type = PostProcess;

	float size;

	DotVFX();
	~DotVFX();

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
