#include "kpch.h"
#include "Mesh.h"

#include "glad/glad.h"

namespace kuai {
	u32 Mesh::meshCounter = 0;

	Mesh::Mesh(const std::vector<Vertex>& vertexData, const std::vector<u32> indices) :
		vertexData(vertexData), indices(indices)
	{
		meshId = meshCounter++;
	}

	Mesh::Mesh(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<u32>& indices)
	{
		u32 vertCount = positions.size() / 3;
		vertexData.resize(vertCount);

		bool useNormals = positions.size() == normals.size();
		
		this->indices = indices;

		for (size_t i = 0; i < vertCount; i++)
		{
			vertexData[i].pos[0] = positions[i * 3];
			vertexData[i].pos[1] = positions[i * 3 + 1];
			vertexData[i].pos[2] = positions[i * 3 + 2];

			if (useNormals)
			{
				vertexData[i].normal[0] = normals[i * 3];
				vertexData[i].normal[1] = normals[i * 3 + 1];
				vertexData[i].normal[2] = normals[i * 3 + 2];
			}

			if (i < texCoords.size())
			{
				vertexData[i].texCoords[0] = texCoords[i * 2];
				vertexData[i].texCoords[1] = texCoords[i * 2 + 1];
			}
		}

		meshId = meshCounter++;
	}

	Mesh::~Mesh()
	{
	}
}