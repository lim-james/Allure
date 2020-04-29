#include "Pixelated.h"

#include "SpriteRenderer.h"
#include <Events/EventsManager.h>	
#include "LoadModel.h"

Pixelated::Pixelated() : size(100.f) {
	shader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/pixelated.frag");
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

	EventsManager::Get()->Subscribe("WINDOW_RESIZE", &Pixelated::ResizeHandler, this);
}

Pixelated::~Pixelated() {
	delete shader;
}

void Pixelated::Initialize() {
	size = 100.f;
}

Component * Pixelated::Clone() const {
	return new Pixelated(*this);
}

void Pixelated::PreRender() {
	fbo->Bind();
}

void Pixelated::PostRender() {
	fbo->Unbind();
}

void Pixelated::Render() {
	shader->Use();
	shader->SetFloat("size", size);

	auto temp = Load::OBJ("Files/Models/cube.obj")->meshes[0];

	glBindVertexArray(VAO);
	//glBindVertexArray(temp->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->GetTexture());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, temp->indicesSize, GL_UNSIGNED_INT, 0);
}

void Pixelated::ResizeHandler(Events::Event * event) {
	fbo->Resize(static_cast<Events::AnyType<vec2i>*>(event)->data);
}
