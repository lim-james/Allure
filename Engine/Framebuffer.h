#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <Math/Vectors.hpp>
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
	Framebuffer(const unsigned& texCount, const unsigned& RBOCount);
	virtual ~Framebuffer();

	virtual void Initialize(const vec2u& size, const std::vector<TextureData>& texs = {}, const std::vector<RenderBufferData>& rbos = {});

	void Bind() const;
	void Unbind() const;

	const unsigned& GetTexture(const unsigned& index = 0) const;
	unsigned GetTextureCount() const;

	const unsigned& GetRBO(const unsigned& index = 0) const;
	unsigned GetRBOCount() const;

	void Resize(const vec2u& resize);
	const vec2u& GetSize() const;

	const unsigned& GetID() const;

};

#endif