#include "CurveDisplay.h"

#include "SpriteRenderer.h"
#include <Events/EventsManager.h>	

CurveDisplay::CurveDisplay() {
	shader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/curve.frag");
	shader->Use();
	shader->SetInt("tex", 0);

	TextureData tData;
	tData.level = 0;
	tData.internalFormat = GL_RGB;
	tData.border = 0;
	tData.format = GL_RGB;
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

	EventsManager::Get()->Subscribe("RESOLUTION_CHANGE", &CurveDisplay::ResolutionHandler, this);
}

CurveDisplay::~CurveDisplay() {
	delete shader;
}

void CurveDisplay::Initialize() {}

Component * CurveDisplay::Clone() const {
	return new CurveDisplay(*this);
}

void CurveDisplay::PreRender() {
	fbo->Bind();
}

void CurveDisplay::PostRender() {
	fbo->Unbind();
}

void CurveDisplay::Render() {
	shader->Use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->GetTexture());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CurveDisplay::ResolutionHandler(Events::Event * event) {
	fbo->Resize(static_cast<Events::AnyType<vec2u>*>(event)->data);
}
