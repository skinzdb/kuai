#include "kpch.h"
#include "Mesh.h"

#include <glad/glad.h>

namespace kuai {
	Mesh::Mesh(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<uint32_t>& indices, std::shared_ptr<Material> material) :
		Mesh(positions, normals, texCoords, indices)
	{
		this->material = material;
	}

	Mesh::Mesh(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<uint32_t>& indices)
	{
		KU_PROFILE_FUNCTION();

		vertCount = indices.size();

		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(4, vboIds);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Positions
		glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		// Normals
		glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		// Texture coords
		glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * vertCount, indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glBindVertexArray(GL_NONE);

		// Default material
		auto tex = std::make_shared<Texture>();
		material = std::make_shared<DefaultMaterial>(tex, tex, 10.0f);
	}

	Mesh::~Mesh()
	{
		// Delete VBOs
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glDeleteBuffers(4, vboIds);

		// Delete VAO
		glBindVertexArray(GL_NONE);
		glDeleteVertexArrays(1, &vaoId);
	}

	void Mesh::render()
	{
		material->render();

		glBindVertexArray(vaoId);
		//glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);

		glDrawElements(GL_TRIANGLES, vertCount, GL_UNSIGNED_INT, 0);

		// Restore state
		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);

		//glBindVertexArray(GL_NONE); // No need to unbind every time
	}

	void Mesh::setTexCoords(std::vector<float>& texCoords)
	{
		glBindVertexArray(vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
	}

}