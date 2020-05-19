#include "LoadFnt.h"
#include "LoadTexture.h"

#include <Helpers/StringHelpers.h>
#include <Logger/Logger.h>
#include <GL/glew.h>

#include <fstream>

Font* Load::FNT(std::string const& fntPath, std::string const& imagePath) {
	if (fontCache[fntPath])
		return fontCache[fntPath];

	std::ifstream ifs(fntPath);

	if (!ifs.is_open()) {
		Debug::Error << '"' << fntPath << "\" not found.\n";
		return nullptr;
	}
	
	std::string buffer;

	// info

	std::getline(ifs, buffer);

	Font* font = new Font;
	font->name = Helpers::Pair(Helpers::Split(buffer, ' ')[1], '=').second;

	// common

	std::getline(ifs, buffer);
	auto common = Helpers::GetDictionary<float>(buffer, ' ', '=');

	const float baseUnit = 1.f / common["base"];
	font->lineHeight = common["lineHeight"] * baseUnit;

	font->scale = vec2f(common["scaleW"], common["scaleH"]);
	const vec2f texSize(font->scale);

	// skip line
	std::getline(ifs, buffer);

	// characters

	std::getline(ifs, buffer);
	auto chars = Helpers::GetDictionary<unsigned>(buffer, ' ', '=');

	font->count = chars["count"];

	std::vector<vec4f> vertices;
	std::vector<unsigned> indices;

	int offset = 0;
	for (unsigned i = 0; i < font->count; ++i) {
		std::getline(ifs, buffer);
		auto charData = Helpers::GetDictionary<float>(buffer, ' ', '=');

		Character character;
		character.index = i;

		// its rectangle in the texture
		const vec4f texRect(
			charData["x"], charData["y"],
			charData["width"], charData["height"]
		);

		const vec2f texOffset(
			charData["xoffset"],
			-charData["yoffset"]
		);

		const float xAdvance = charData["xadvance"];

		character.rect.size = texRect.size * baseUnit;
		character.rect.origin = texOffset * baseUnit;
		character.xAdvance = xAdvance * baseUnit;

		font->characters[static_cast<int>(charData["id"])] = character;

		vec4f uvRect = texRect / vec4f(texSize, texSize);
		uvRect.y = 1.f - uvRect.y;

		vec4f v;
		v = vec4f(
			0.f, 
			-character.rect.size.h,
			uvRect.origin.u,
			uvRect.origin.v - uvRect.size.h
		);
		vertices.push_back(v);

		v = vec4f(
			character.rect.size.w, 
			-character.rect.size.h,
			uvRect.origin.u + uvRect.size.w,
			uvRect.origin.v - uvRect.size.h
		);
		vertices.push_back(v);

		v = vec4f(
			character.rect.size.w, 
			0.f,
			uvRect.origin.u + uvRect.size.w,
			uvRect.origin.v
		);
		vertices.push_back(v);

		v = vec4f(vec2f(0.f), uvRect.origin);
		vertices.push_back(v);

		indices.push_back(offset + 0);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 0);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);
		offset += 4;
	}

	ifs.close();

	font->texture = Load::Texture2D(imagePath);

	unsigned VBO, EBO;
	glGenVertexArrays(1, &font->VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(font->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec4f), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	unsigned unit = sizeof(vec2f);
	unsigned stride = sizeof(vec4f);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(unit));
	glEnableVertexAttribArray(1);
	
	fontCache[fntPath] = font;

	return font;
}

void Load::ClearFontCache() {
	for (auto& item : fontCache)
		delete item.second;

	fontCache.clear();
}
