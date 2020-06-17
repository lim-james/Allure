#ifndef BLOOM_H
#define BLOOM_H

#include "PostProcess.h"

#include "Framebuffer.h"
#include "Shader.h"

#include <Events/Event.h>

struct Bloom : PostProcess {

	float unit;
	unsigned amount;

	Bloom();
	~Bloom();

	void Initialize() override;
	Component* Clone() const override;

	void PreRender() override;
	void PostRender() override;
	void Render() override;

private:

	Shader* blurShader;
	Shader* additiveShader;

	Framebuffer* fbo;
	Framebuffer *blurPass, *finalBloomPass;

	unsigned bloomTexture;

	void ResolutionHandler(Events::Event* event);

};

#endif
