#include "Bloom.h"

#include <Events/EventsManager.h>

Bloom::Bloom() {
	blurShader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/blur.frag"); 
	blurShader->Use();
	blurShader->SetInt("tex", 0);

	additiveShader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/bloom.frag");
	additiveShader->Use();
	additiveShader->SetInt("tex0", 0);
	additiveShader->SetInt("tex1", 1);

	{
		std::vector<TextureData> tDataList;

		for (int i = 0; i < 2; ++i) {
			TextureData tData;
			tData.level = 0;
			tData.internalFormat = GL_RGB16F;
			tData.border = 0;
			tData.format = GL_RGBA;
			tData.type = GL_UNSIGNED_BYTE;
			tData.attachment = GL_COLOR_ATTACHMENT0 + i;
			tData.parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
			tData.parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
			tData.parameters.push_back({ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE });
			tData.parameters.push_back({ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE });
			tDataList.push_back(tData);
		}

		RenderBufferData rbData;
		rbData.internalFormat = GL_DEPTH24_STENCIL8;
		rbData.attachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;

		fbo = new Framebuffer(2, 1);
		fbo->Initialize(vec2u(1600, 900), tDataList, { rbData });
	}

	{
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

		blurPass = new Framebuffer(1, 1);
		blurPass->Initialize(vec2u(1600, 900), { tData }, { rbData });

		finalBloomPass = new Framebuffer(1, 1);
		finalBloomPass->Initialize(vec2u(1600, 900), { tData }, { rbData });
	}

	EventsManager::Get()->Subscribe("RESOLUTION_CHANGE", &Bloom::ResolutionHandler, this);
}

Bloom::~Bloom() {
	delete blurShader;
	delete additiveShader;
}

void Bloom::Initialize() {}

Component * Bloom::Clone() const {
	return new Bloom(*this);
}

void Bloom::PreRender() {
	fbo->Bind();
}

void Bloom::PostRender() {
	fbo->Unbind();

	Framebuffer* fb[2] = { blurPass, finalBloomPass };

	bool horizontal = true, firstIteration = true;
	unsigned amount = 10;

	blurShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	for (unsigned i = 0; i < amount; ++i) {
		fb[horizontal]->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const unsigned texture = firstIteration ? fbo->GetTexture(1) : fb[!horizontal]->GetTexture();
		blurShader->SetInt("horizontal", horizontal);

		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		horizontal = !horizontal;
		if (firstIteration)
			firstIteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bloomTexture = fb[horizontal]->GetTexture();
}

void Bloom::Render() {
	additiveShader->Use();
	additiveShader->SetFloat("exposure", 1.f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo->GetTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Bloom::ResolutionHandler(Events::Event * event) {
	const vec2u resolution = static_cast<Events::AnyType<vec2u>*>(event)->data;
	fbo->Resize(resolution);
	blurPass->Resize(resolution);
	finalBloomPass->Resize(resolution);
}
