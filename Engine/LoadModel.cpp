#include "LoadModel.h"

#include <Math/Vectors.h>

#include <Logger/Logger.h>
#include <Helpers/StringHelpers.h>
// asssimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <string>

Model* Load::OBJ(std::string const& filepath) {
	if (modelCache.find(filepath) != modelCache.end())
		return modelCache[filepath];

	std::ifstream ifs(filepath);

	std::string groupName;
	std::string buffer;

	std::vector<vec3f> positions;
	std::vector<vec2f> texCoords;
	std::vector<vec3f> normals;

	Vertex vertexBuffer;

	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	while (!ifs.eof()) {
		std::getline(ifs, buffer);
		auto list = Helpers::Split(buffer, ' ');

		if (list.size() > 0) {
			if (list[0] == "g") {
				list.erase(list.begin());
				groupName = Helpers::Join(list, ' ');
			} else if (list[0] == "v") {
				positions.push_back(vec3f(
					Helpers::Parse<float>(list[1]),
					Helpers::Parse<float>(list[2]),
					Helpers::Parse<float>(list[3])
				));
			} else if (list[0] == "vt") {
				texCoords.push_back(vec2f(
					Helpers::Parse<float>(list[1]),
					Helpers::Parse<float>(list[2])
				));
			} else if (list[0] == "vn") {
				normals.push_back(vec3f(
					Helpers::Parse<float>(list[1]),
					Helpers::Parse<float>(list[2]),
					Helpers::Parse<float>(list[3])
				));
			} else if (list[0] == "f") {
				std::vector<int> listBuffer;

				const auto& order = list.size() == 5 ? quadIndices : triangleIndices;

				for (const auto& i : order) {
					listBuffer = Helpers::Split<int>(list[i], '/');

					const auto position = positions[listBuffer[0] - 1];

					vec3f normal = vec3f(0.0f);
					if (listBuffer[2] >= 0 && listBuffer[2] - 1 < static_cast<int>(normals.size()))
						normal = normals[listBuffer[2] - 1];

					vec3f texCoord = vec3f(0.0f);
					if (listBuffer[1] >= 0 && listBuffer[1] - 1 < static_cast<int>(texCoords.size()))
						texCoord = texCoords[listBuffer[1] - 1];

					vertexBuffer = { position, normal, texCoord };

					const auto vertexIndex = std::find(vertices.begin(), vertices.end(), vertexBuffer);

					if (vertexIndex == vertices.end()) {
						const unsigned index = vertices.size();
						vertices.push_back(vertexBuffer);
						indices.push_back(index);
					} else {
						indices.push_back(std::distance(vertices.begin(), vertexIndex));
					}
				}
			}
		}
	}

	ifs.close();

	Mesh* mesh = new Mesh(vertices, indices);
	Model* model = new Model({ mesh });
	modelCache[filepath] = model;

	return model;
}

//Model * Load::Meshes(std::string const & filepath) {
//	if (modelCache.find(filepath) != modelCache.end())
//		return modelCache[filepath];
//
//	Mesh* mesh = new Mesh(vertices, indices);
//	Model* model = new Model({ mesh });
//	modelCache[filepath] = model;
//
//	return model;
//}

void Load::ClearModelCache() {
	for (const auto& model : modelCache)
		delete model.second;

	modelCache.clear();
}