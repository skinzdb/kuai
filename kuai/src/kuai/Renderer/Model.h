#pragma once

#include "Mesh.h"
#include "Texture.h"

#define OPENDDL_STATIC_LIBARY

// Forward Declarations
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace kuai {
	/** \class Model
	*   \brief A 3D object that is comprised of a collection of meshes.
	*/
	class Model
	{
	public:
		/**
		* Load model from a 3D object file.
		*/
		Model(const std::string& filename);
		/**
		* Create model by specifying a singlular mesh.
		*/
		Model(std::shared_ptr<Mesh> mesh);

		/// @private
		void render();

		/**
		* Returns list of meshes this model is made of.
		*/
		std::vector<std::shared_ptr<Mesh>> getMeshes() { return meshes; }

	private:
		void processNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, uint64_t type);

	private:
		std::vector<std::shared_ptr<Mesh>> meshes;
		std::string directory;
		std::unordered_map<std::string, Texture*> loadedTexMap;
	};
}