#ifndef HEX_H
#define HEX_H

#include <Math/Vectors.hpp>

#define UP			0
#define UP_RIGHT	1
#define DOWN_RIGHT	2
#define DOWN		3
#define DOWN_LEFT	4
#define UP_LEFT		5
#define HEX_DIR_COUNT	6

struct Hex {

	vec3f position;
	
	Hex * neighbours[HEX_DIR_COUNT];

};

#endif
