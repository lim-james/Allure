#include "Framebuffer.h"

#include <Math/Math.hpp>
#include <GL/glew.h>

Framebuffer::Framebuffer()
	: id(0)
	, size(0) {}

Framebuffer::Framebuffer(const unsigned& texCount, const unsigned& RBOCount) {
	glGenFramebuffers(1, &id);

	for (unsigned i = 0; i < texCount; ++i) {
		unsigned texture;
		glGenTextures(1, &texture);
		textures.push_back(texture);
	}

	for (unsigned i = 0; i < texCount; ++i) {
		unsigned rbo;
		glGenRenderbuffers(1, &rbo);
		RBOs.push_back(rbo);
	}
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &id);
	textures.clear();
}

void Framebuffer::Initialize(const vec2u& size, const std::vector<TextureData>& texs, const std::vector<RenderBufferData>& rbos) {
	this->texturesData = texs;
	this->rbosData = rbos;

	Resize(size);
}


void Framebuffer::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const unsigned& Framebuffer::GetTexture(const unsigned& index) const {
	return textures[index];
}

unsigned Framebuffer::GetTextureCount() const {
	return textures.size();
}

const unsigned& Framebuffer::GetRBO(const unsigned& index) const {
	return RBOs[index];
}

unsigned Framebuffer::GetRBOCount() const {
	return RBOs.size();
}

void Framebuffer::Resize(const vec2u& resize) {
	this->size = resize;

	glBindFramebuffer(GL_FRAMEBUFFER, id);

	const unsigned texCount = Math::Min(texturesData.size(), textures.size());
	const unsigned rboCount = Math::Min(rbosData.size(), RBOs.size());

	std::vector<unsigned> attachments;

	for (unsigned i = 0; i < texCount; ++i) {
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		const auto& t = texturesData[i];

		glTexImage2D(
			GL_TEXTURE_2D,
			t.level,
			t.internalFormat,
			size.w,
			size.h,
			t.border,
			t.format,
			t.type,
			nullptr
		);

		for (const auto& p : t.parameters) {
			glTexParameteri(GL_TEXTURE_2D, p.name, p.param);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, t.attachment, GL_TEXTURE_2D, textures[i], 0);
		attachments.push_back(t.attachment);
	}

	glDrawBuffers(texCount, &attachments[0]);

	for (unsigned i = 0; i < rboCount; ++i) {
		glBindRenderbuffer(GL_RENDERBUFFER, RBOs[i]);
		const auto& r = rbosData[i];

		glRenderbufferStorage(GL_RENDERBUFFER, r.internalFormat, size.w, size.h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, r.attachmentFormat, GL_RENDERBUFFER, RBOs[i]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const vec2u& Framebuffer::GetSize() const {
	return size;
}

const unsigned& Framebuffer::GetID() const {
	return id;
}