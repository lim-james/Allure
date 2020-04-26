#ifndef INVERT_PROCESS_H
#define INVERT_PROCESS_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct CurveDisplay : PostProcess {

	CurveDisplay();

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
