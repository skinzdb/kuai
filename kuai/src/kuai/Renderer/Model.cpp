#include "kpch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace kuai {

	Model::Model(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			KU_CORE_ERROR("Error loading model: {0}", importer.GetErrorString());
			return;
		}
		directory = filename.substr(0, filename.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	Model::Model(std::shared_ptr<Mesh> mesh)
	{
		meshes.push_back(mesh);
	}

	void Model::render()
	{
		for (auto& mesh : meshes)
			mesh->render();
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<float> vertices((size_t)mesh->mNumVertices * 3);
		std::vector<float> normals((size_t)mesh->mNumVertices * 3);
		std::vector<float> texCoords((size_t)mesh->mNumVertices * 2);
		std::vector<uint32_t> indices;
		std::vector<Texture> textures;

		// Vertices and Normals
		memcpy(vertices.data(), &mesh->mVertices[0], mesh->mNumVertices * sizeof(float) * 3);
		memcpy(normals.data(), &mesh->mNormals[0], mesh->mNumVertices * sizeof(float) * 3);

		// Texture Coords
		if (mesh->mTextureCoords[0])
		{
			memcpy(texCoords.data(), &mesh->mTextureCoords[0][0], mesh->mNumVertices * sizeof(float) * 2);
		}

		// Indices
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Material
		if (mesh->mMaterialIndex)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			// TODO: add loaded textures to material
		}

		return std::make_shared<Mesh>(vertices, normals, texCoords, indices);
	}

	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType& type)
	{
		std::vector<Texture> textures;

		for (size_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string filename = directory + str.C_Str();

			if (loadedTexMap.find(filename) == loadedTexMap.end())
			{
				textures.push_back(loadedTexMap[filename]);
			}
			else
			{
				Texture tex(directory + str.C_Str());
				textures.push_back(tex);
				loadedTexMap.insert(std::pair<std::string, Texture>(filename, tex));
			}
		}

		return textures;
	}

}
