#include "smpch.h"
#include "Mesh.h"

#include <glad/glad.h>

namespace Smong {
	Mesh::Mesh(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<uint32_t>& indices)
	{
		vertCount = indices.size();

		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(4, vboIds);

		// Positions
		glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);
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
	}

	Mesh::~Mesh()
	{
		glDisableVertexAttribArray(GL_NONE);

		// Delete VBOs
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glDeleteBuffers(4, vboIds);

		// Delete VAO
		glBindVertexArray(GL_NONE);
		glDeleteVertexArrays(1, &vaoId);
	}

	void Mesh::Render()
	{
		glBindVertexArray(vaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glDrawElements(GL_TRIANGLES, vertCount, GL_UNSIGNED_INT, 0);

		// Restore state
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//glBindVertexArray(GL_NONE); // No need to unbind every time
		//glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

}