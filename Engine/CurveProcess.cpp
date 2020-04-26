#include "CurveProcess.h"

#include "SpriteRenderer.h"
#include <Events/EventsManager.h>	

CurveProcess::CurveProcess() {
	shader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/curve.frag");
	shader->Use();
	shader->SetInt("tex", 0);

	TextureData tData;
	tData.level = 0;
	tData.internalFormat = GL_RGB16F;
	tData.border = 0;
	tData.format = GL_RGBA;
	tData.type = GL_UNSIGNED_BYTE;
	tData.attachment = GL_COLOR_ATTACHMENT0;
	tData.parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
	tData.parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
	tData.parameters.push_back({ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE });
	tData.parameters.push_back({ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE });

	RenderBufferData rbData;
	rbData.internalFormat = GL_DEPTH24_STENCIL8;
	rbData.attachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;

	fbo = new Framebuffer(1, 1);
	fbo->Initialize(vec2u(1600, 900), { tData }, { rbData });

	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &CurveProcess::ResizeHandler, this);
}

void CurveProcess::PreRender() {
	fbo->Bind();
}

void CurveProcess::PostRender() {
	fbo->Unbind();
}

void CurveProcess::ResizeHandler(Events::Event * event) {
	fbo->Resize(static_cast<Events::AnyType<vec2i>*>(event)->data);
}

void CurveProcess::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->GetTexture());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
