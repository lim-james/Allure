#include "LoadTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <Logger/Logger.h>
#include <GL/glew.h>
#include <fstream>

unsigned Load::Texture2D(std::string const & filepath, bool const& flipVertically) {
	if (textureCache.find(filepath) != textureCache.end())
		return textureCache[filepath];

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		textureCache[filepath] = texture;
	} else {
		Debug::Error << "Failed to load texture \"" << filepath <<  "\"\n";
	}

	stbi_image_free(data);
	return texture;
}

unsigned Load::Cubemap(std::vector<std::string> const& filepaths) {
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int width, height, nrChannels;
	for (unsigned i = 0; i < filepaths.size(); i++) {
		unsigned char *data = stbi_load(filepaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			const unsigned target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			if (nrChannels == 4)
				glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else if (nrChannels == 3)
				glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else
				glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		} else {
			Debug::Error << "Cubemap tex failed to load at path: " << filepaths[i] << '\n';
		}
		stbi_image_free(data);
	}

	return texture;
}

unsigned Load::TGA(std::string const& filepath) {
	if (textureCache.find(filepath) != textureCache.end())
		return textureCache[filepath];

	std::ifstream fileStream(filepath, std::ios::binary);
	if (!fileStream.is_open()) {
		Debug::Error << "Impossible to open " << filepath << ". Are you in the right directory ?\n";
		return 0;
	}

	GLubyte		header[18];									// first 6 useful header bytes
	GLuint		bytesPerPixel;								  // number of bytes per pixel in TGA gile
	GLuint		imageSize;									  // for setting memory
	GLubyte* data;
	GLuint		texture = 0;
	unsigned	width, height;

	fileStream.read((char*)header, 18);
	width = header[12] + header[13] * 256;
	height = header[14] + header[15] * 256;

	if (width <= 0 ||								// is width <= 0
		height <= 0 ||								// is height <=0
		(header[16] != 24 && header[16] != 32))		// is TGA 24 or 32 Bit
	{
		fileStream.close();							// close file on failure
		Debug::Error << "\"" << filepath << "\" has invalid encoding.\n";
		return 0;
	}

	bytesPerPixel = header[16] / 8;						//divide by 8 to get bytes per pixel
	imageSize = width * height * bytesPerPixel;	// calculate memory required for TGA data

	data = new GLubyte[imageSize];
	fileStream.seekg(18, std::ios::beg);
	fileStream.read((char*)data, imageSize);
	fileStream.close();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (bytesPerPixel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	else //bytesPerPixel == 4
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	delete[] data;

	textureCache[filepath] = texture;
	return texture;
}
