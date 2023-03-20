#pragma once

#include "Mesh.h"
#include "Texture.h"

#define OPENDDL_STATIC_LIBARY

// Forward Declarations
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType : unsigned int;

namespace kuai {
	class Model
	{
	public:
		Model(const std::string& filename);
		Model(std::shared_ptr<Mesh> mesh);

		void render();

		std::vector<std::shared_ptr<Mesh>> getMeshes() { return meshes; }

	private:
		void processNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, const aiTextureType& type);

	private:
		std::vector<std::shared_ptr<Mesh>> meshes;
		std::string directory;
		std::unordered_map<std::string, Texture> loadedTexMap;
	};
}