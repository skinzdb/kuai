#include "smpch.h"
#include "Mesh.h"

#include <glad/glad.h>

namespace Smong {
	Mesh::Mesh(float positions[], uint32_t posCount, float texCoords[], uint32_t texCount, uint32_t indices[], uint32_t vertCount) : vertCount(vertCount)
	{
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(3, vboIds);

		// Positions
		glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions) * posCount, positions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		// Texture coords
		glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords) * texCount, texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * vertCount, indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glBindVertexArray(GL_NONE);
	}

	Mesh::~Mesh()
	{
		glDisableVertexAttribArray(GL_NONE);

		// Delete VBOs
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glDeleteBuffers(3, vboIds);

		// Delete VAO
		glBindVertexArray(GL_NONE);
		glDeleteVertexArrays(1, &vaoId);
	}

	void Mesh::Render()
	{
		glBindVertexArray(vaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, vertCount, GL_UNSIGNED_INT, 0);

		// Restore state
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//glBindVertexArray(GL_NONE); // No need to unbind every time
		//glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

}