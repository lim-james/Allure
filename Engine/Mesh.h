#ifndef MESH_H
#define MESH_H

#include "Vertex.h"

#include <Math/Mat4.hpp>
#include <vector>

struct Mesh {

	unsigned VAO;
	unsigned indicesSize;
	mat4f modelTransform;

	Mesh(std::vector<Vertex> const& vertices, std::vector<unsigned> const& indices);
	~Mesh();

	unsigned GenerateVAO() const;

private:

	const std::vector<Vertex> vertices;
	const std::vector<unsigned> indices;

};

#endif
