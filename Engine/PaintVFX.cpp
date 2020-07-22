#include "PaintVFX.h"

#include "SpriteRenderer.h"
#include <Events/EventsManager.h>	
#include "LoadModel.h"

PaintVFX::PaintVFX() : unit(1.f), radius(3.f) {
	shader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/paint.frag");
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

	EventsManager::Get()->Subscribe("RESOLUTION_CHANGE", &PaintVFX::ResolutionHandler, this);
}

PaintVFX::~PaintVFX() {
	delete shader;
}

void PaintVFX::Initialize() {
	unit = 1.f;
	radius = 3.f;
}

Component * PaintVFX::Clone() const {
	return new PaintVFX(*this);
}

void PaintVFX::PreRender() {
	fbo->Bind();
}

void PaintVFX::PostRender() {
	fbo->Unbind();
}

void PaintVFX::Render() {
	shader->Use();
	shader->SetFloat("unit", unit);
	shader->SetFloat("radius", radius);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->GetTexture());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PaintVFX::Render(unsigned const & tex) {
	shader->Use();
	shader->SetFloat("unit", unit);
	shader->SetFloat("radius", radius);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PaintVFX::ResolutionHandler(Events::Event * event) {
	fbo->Resize(static_cast<Events::AnyType<vec2u>*>(event)->data);
}
