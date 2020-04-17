#include "Mesh.h"

#include <GL/glew.h>

Mesh::Mesh(std::vector<Vertex> const& vertices, std::vector<unsigned> const& indices) 
	: vertices(vertices) 
	, indices(indices) {
	indicesSize = indices.size();

	unsigned VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	unsigned unit = sizeof(vec3f);
	unsigned stride = sizeof(Vertex);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(unit));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * unit));
	glEnableVertexAttribArray(2);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
}

unsigned Mesh::GenerateVAO() const {
	unsigned newVAO, VBO, EBO;

	glGenVertexArrays(1, &newVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(newVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	unsigned unit = sizeof(vec3f);
	unsigned stride = sizeof(Vertex);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(unit));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * unit));
	glEnableVertexAttribArray(2);

	return newVAO;
}
