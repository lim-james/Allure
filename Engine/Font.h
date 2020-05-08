#ifndef FONT_H
#define FONT_H

#include <Math/Vectors.h>
#include <Encoding/Resource.h>

#include <string>
#include <map>

// values are relative to base height
struct Character {
	unsigned index;
	vec4f rect;
	float xAdvance;
};

struct Font : Resource {

	std::string name;
	vec2f scale;
	// relative to base height
	float lineHeight;

	unsigned count;
	std::map<int, Character> characters;

	unsigned texture;
	unsigned VAO;

};

#endif
