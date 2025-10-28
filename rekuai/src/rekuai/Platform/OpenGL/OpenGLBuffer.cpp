#include "OpenGLBuffer.h"

#include "rekuai/Core/Core.h"
#include "rekuai/Renderer/Buffer.h"

#include "glad/glad.h"

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
		default:
			KU_CORE_ASSERT(false, "Unknown shader data type");
			return 0;
		}
	}

	// Vertex Buffer *********************************************************

	OpenGLBuffer::OpenGLBuffer(uint32_t size)
	{
		glCreateBuffers(1, &buf_id);
		glNamedBufferStorage(buf_id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	OpenGLBuffer::~OpenGLBuffer()
	{
		glDeleteBuffers(1, &buf_id);
	}

	void OpenGLBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, buf_id);
	}

	void OpenGLBuffer::set_data(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buf_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void OpenGLBuffer::reset(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buf_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	// Index Buffer ***********************************************************

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count) : count(count)
	{
		glCreateBuffers(1, &buf_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &buf_id);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf_id);
	}

	// Indirect Buffer ********************************************************

	OpenGLIndirectBuffer::OpenGLIndirectBuffer(const std::vector<IndirectCommand>& commands)
	{
		glCreateBuffers(1, &buf_id);
		count = commands.size();
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buf_id);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(IndirectCommand) * count, commands.data(), GL_STATIC_DRAW);
	}

	OpenGLIndirectBuffer::~OpenGLIndirectBuffer()
	{
		glDeleteBuffers(1, &buf_id);
	}

	void OpenGLIndirectBuffer::bind() const
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buf_id);
	}

	// Vertex Array ***********************************************************

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &vao_id);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &vao_id);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(vao_id);
	}

	void OpenGLVertexArray::add_vertex_buffer(std::shared_ptr<VertexBuffer> buf)
	{
		KU_CORE_ASSERT(buf->get_layout().get_elements().size(), "Vertex buffer has no layout.");

		glBindVertexArray(vao_id);
		buf->bind();

		auto &layout = buf->get_layout();
		for (auto &element : layout)
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
					reinterpret_cast<const void *>(element.offset)
				);
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
					reinterpret_cast<const void *>(element.offset)
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
						reinterpret_cast<const void *>(element.offset + sizeof(float) * count * i)
					);
					glVertexAttribDivisor(index, 1); // Tells vertex attribute to increment once per instance instead of per vertex
					index++;
				}
			}
			case ShaderDataType::NONE:
    			KU_CORE_ASSERT(false, "Unknown shader data type");
			}
		}

		vertex_bufs.push_back(buf);
	}

	void OpenGLVertexArray::set_index_buffer(std::shared_ptr<IndexBuffer> buf)
	{
		glBindVertexArray(vao_id);
		buf->bind();

		index_buf = buf;
	}
}
