#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

struct Model {

	std::vector<Mesh*> meshes;

	Model(const std::vector<Mesh*>& meshes);
	~Model();

};

#endif
