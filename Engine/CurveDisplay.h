#ifndef CURVE_DISPLAY_H
#define CURVE_DISPLAY_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct CurveDisplay : PostProcess {

	CurveDisplay();
	~CurveDisplay();

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
