#ifndef LOAD_MODEL_H
#define LOAD_MODEL_H

#include "Model.h"

#include <Math/Mat4.hpp>
// asssimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <string>

namespace Load {

	static const std::vector<int> triangleIndices = { 1, 2, 3 };
	static const std::vector<int> quadIndices = { 1, 2, 3, 3, 4, 1 };

	static std::map<std::string, Model*> modelCache;

	Model* OBJ(std::string const& filepath);
	Model* Meshes(std::string const& filepath);

	void ProcessNode(aiNode *node, const aiScene * scene, Model* const model, aiMatrix4x4 const& transform);
	Mesh* ProcessMesh(aiMesh *mesh, const aiScene *scene, Model* const model);
	void LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string const& typeName, Model* const model, Mesh* const mesh);

	void ClearModelCache();

}

#endif
