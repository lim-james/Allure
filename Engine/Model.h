#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <map>

struct Model {

	std::string directory;
	std::vector<Mesh*> meshes;
	std::map<Mesh*, std::vector<unsigned>> textures;
	std::map<Mesh*, vec3f> worldPositions;

	Model();
	Model(const std::vector<Mesh*>& meshes);
	~Model();

};

#endif
