#include "Model.h"

Model::Model(std::vector<Mesh*> const& meshes)
	: meshes(meshes) {}

Model::~Model() {
	for (auto const& mesh : meshes)
		delete mesh;

	meshes.clear();
}