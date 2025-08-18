#include "rekuai/Core/Core.h"
#include "Buffer.h"

#include "glad/glad.h"
#include <cstddef>

namespace kuai {

	static GLenum get_opengl_type(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::INT:
			return GL_INT;
		case ShaderDataType::FLOAT:
		case ShaderDataType::VEC2:
		case ShaderDataType::VEC3:
		case ShaderDataType::VEC4:
		case ShaderDataType::MAT3:
		case ShaderDataType::MAT4:
			return GL_FLOAT;
		}

		KU_CORE_ASSERT(false, "Unknown shader data type.");
		return 0;
	}

	// Vertex Buffer *********************************************************

	VertexBuffer::VertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &buf_id);
		glBindBuffer(GL_ARRAY_BUFFER, buf_id);
		glBufferData(buf_id, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const float* vertices, uint32_t size, DrawHint drawHint)
	{
		glCreateBuffers(1, &buf_id);
		glBindBuffer(GL_ARRAY_BUFFER, buf_id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, drawHint == DrawHint::STATIC ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &buf_id);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, buf_id);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::set_data(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buf_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::reset(const void* data, uint32_t size, DrawHint drawHint)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buf_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, drawHint == DrawHint::STATIC ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	// Index Buffer ***********************************************************

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : count(count)
	{
		glCreateBuffers(1, &buf_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &buf_id);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_id);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Indirect Buffer ********************************************************

	IndirectBuffer::IndirectBuffer(const std::vector<IndirectCommand>& commands)
	{
		glCreateBuffers(1, &buf_id);
		count = commands.size();
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buf_id);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(IndirectCommand) * count, commands.data(), GL_STATIC_DRAW);
	}

	IndirectBuffer::~IndirectBuffer()
	{
		glDeleteBuffers(1, &buf_id);
	}

	void IndirectBuffer::bind() const
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buf_id);
	}

	void IndirectBuffer::unbind() const
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	}

	// Vertex Array ***********************************************************

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &vao_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &vao_id);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(vao_id);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::add_vertex_buffer(std::unique_ptr<VertexBuffer> buf)
	{
		KU_CORE_ASSERT(buf->get_layout().get_elements().size(), "Vertex buffer has no layout.");

		glBindVertexArray(vao_id);
		buf->bind();

		auto& layout = buf->get_layout();
		for (auto& element : layout)
		{
			switch (element.type)
			{
			case ShaderDataType::INT:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(
					index,
					element.get_component_count(),
					GL_INT,
					layout.get_stride(),
					(const void*)element.offset);
				index++;
				break;
			}
			case ShaderDataType::FLOAT:
			case ShaderDataType::VEC2:
			case ShaderDataType::VEC3:
			case ShaderDataType::VEC4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(
					index,
					element.get_component_count(),
					GL_FLOAT,
					GL_FALSE,
					layout.get_stride(),
					(const void*)element.offset
				);
				index++;
				break;
			}
			case ShaderDataType::MAT3:
			case ShaderDataType::MAT4:
			{
				// The maximum size of a vertex attribute is 4 elements (vec4), so need to assign mutiple vertex attribute slots for a matrix
				uint8_t count = element.get_component_count();
				for (size_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						count,
						GL_FLOAT,
						GL_FALSE,
						layout.get_stride(),
						(const void*)(element.offset + sizeof(float) * count * i)
					);
					glVertexAttribDivisor(index, 1); // Tells vertex attribute to increment once per instance instead of per vertex
					index++;
				}
			}
			}
		}

		vertex_bufs.push_back(std::move(buf));
	}

	void VertexArray::set_index_buffer(std::unique_ptr<IndexBuffer> buf)
	{
		glBindVertexArray(vao_id);
		buf->bind();

		index_buf = std::move(buf);
	}
}
