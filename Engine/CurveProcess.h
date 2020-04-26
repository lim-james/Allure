#ifndef INVERT_PROCESS_H
#define INVERT_PROCESS_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct CurveProcess : PostProcess {

	CurveProcess();

	void PreRender() override;
	void PostRender() override;
	
private:

	Shader* shader;
	Framebuffer* fbo;

	void ResizeHandler(Events::Event* event);

	void Render() override;

};

#endif
