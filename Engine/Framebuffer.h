#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <Math/Vectors.h>
#include <GL/glew.h>

#include <vector>

struct ParameterData {
	GLenum name;
	GLint param;
};

struct TextureData {
	GLint level;
	GLint internalFormat;
	GLint border;
	GLenum format;
	GLenum type;

	GLenum attachment;

	std::vector<ParameterData> parameters;
};

struct RenderBufferData {
	GLenum internalFormat;
	GLenum attachmentFormat;
};

class Framebuffer {

	unsigned id;
	vec2u size;

	std::vector<unsigned> textures;
	std::vector<TextureData> texturesData;

	std::vector<unsigned> RBOs;
	std::vector<RenderBufferData> rbosData;

public:

	Framebuffer();
	Framebuffer(unsigned const& texCount, unsigned const& RBOCount);
	virtual ~Framebuffer();

	virtual void Initialize(vec2u const& size, std::vector<TextureData> const& texs = {}, std::vector<RenderBufferData> const& rbos = {});

	void Bind() const;
	void Unbind() const;

	unsigned const& GetTexture(unsigned const& index = 0) const;
	unsigned GetTextureCount() const;

	unsigned const& GetRBO(unsigned const& index = 0) const;
	unsigned GetRBOCount() const;

	void Resize(vec2u const& resize);
	vec2u const& GetSize() const;

	unsigned const& GetID() const;

};

#endif