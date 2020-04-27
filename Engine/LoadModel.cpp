#include "LoadModel.h"

#include  "LoadTexture.h"

#include <Math/Vectors.h>
#include <Logger/Logger.h>
#include <Helpers/StringHelpers.h>

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

Model * Load::Meshes(std::string const & filepath) {
	if (modelCache.find(filepath) != modelCache.end())
		return modelCache[filepath];

	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		Debug::Error << "ERROR::ASSIMP:: " << importer.GetErrorString() << '\n';
		return nullptr;
	}

	Model* const model = new Model;
	modelCache[filepath] = model;
	model->directory = filepath.substr(0, filepath.find_last_of('/'));;

	ProcessNode(scene->mRootNode, scene, model, scene->mRootNode->mTransformation);

	return model;
}

void Load::ProcessNode(aiNode *node, const aiScene * scene, Model* const model, aiMatrix4x4 const& transform) {
	for (unsigned i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* const result = ProcessMesh(mesh, scene, model);
		result->modelTransform = mat4f(
			transform.a1, transform.b1, transform.c1, transform.d1,
			transform.a2, transform.b2, transform.c2, transform.d2,
			transform.a3, transform.b3, transform.c3, transform.d3,
			transform.a4, transform.b4, transform.c4, transform.d4
		);
		model->meshes.push_back(result);
	}
	
	for (unsigned i = 0; i < node->mNumChildren; ++i)
		ProcessNode(node->mChildren[i], scene, model, transform * node->mChildren[i]->mTransformation);
}


Mesh* Load::ProcessMesh(aiMesh *mesh, const aiScene *scene, Model* const model) {
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		vec3f vector; 
	
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
	
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
	
		if (mesh->mTextureCoords[0]) {
			vec2f vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		} else {
			vertex.texCoords = 0.f;
		}

		//vector.x = mesh->mTangents[i].x;
		//vector.y = mesh->mTangents[i].y;
		//vector.z = mesh->mTangents[i].z;
		//vertex.Tangent = vector;

		//vector.x = mesh->mBitangents[i].x;
		//vector.y = mesh->mBitangents[i].y;
		//vector.z = mesh->mBitangents[i].z;
		//vertex.Bitangent = vector;

		vertices.push_back(vertex);
	}
	
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	Mesh* const result = new Mesh(vertices, indices);

	LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", model, result);
	LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", model, result);
	LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", model, result);
	LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", model, result);

	return result;
}

void Load::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string const& typeName, Model* const model, Mesh* const mesh) {
	auto& textures = model->textures[mesh];

	for (unsigned i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);
		Debug::Warn << model->directory << '\n';
		textures.push_back(Load::Texture2D(model->directory + str.C_Str()));
	}

}

void Load::ClearModelCache() {
	for (const auto& model : modelCache)
		delete model.second;

	modelCache.clear();
}