#pragma once

#include <memory>
#include "rekuai/Core/Core.h"

namespace kuai {

    enum class ShaderDataType
    {
        NONE = 0, INT, FLOAT, VEC2, VEC3, VEC4, MAT3, MAT4
    };

    static uint32_t sizeOfShaderDataType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::INT:   return 4;
        case ShaderDataType::FLOAT: return 4;
        case ShaderDataType::VEC2:  return 8;
        case ShaderDataType::VEC3:  return 12;
        case ShaderDataType::VEC4:  return 16;
        case ShaderDataType::MAT3:  return 36;
        case ShaderDataType::MAT4:  return 64;
        default:
            KU_CORE_ASSERT(false, "Unknown shader data type.");
            return 0;
        }
    }

    struct BufferElement
    {
        std::string name;
        ShaderDataType type;
        uint32_t size;
        uint32_t offset;

        BufferElement() = default;

        BufferElement(ShaderDataType type, const std::string& name) :
            name(name), type(type), size(sizeOfShaderDataType(type)), offset(0) {}

        uint32_t get_component_count() const
        {
            switch (type)
            {
            case ShaderDataType::INT:   return 1;
            case ShaderDataType::FLOAT: return 1;
            case ShaderDataType::VEC2:  return 2;
            case ShaderDataType::VEC3:  return 3;
            case ShaderDataType::VEC4:  return 4;
            case ShaderDataType::MAT3:  return 3; // (3 * Vec3)
            case ShaderDataType::MAT4:  return 4; // (4 * Vec4)
            case ShaderDataType::NONE:  return 0;
            }
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;

        BufferLayout(std::initializer_list<BufferElement> elements) :
            elements(elements)
        {
            calc_offsets_and_stride();
        }

        uint32_t get_stride() const { return stride; }
        const std::vector<BufferElement>& get_elements() const { return elements; }

        std::vector<BufferElement>::iterator begin() { return elements.begin(); }
        std::vector<BufferElement>::iterator end() { return elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

    private:
        void calc_offsets_and_stride()
        {
            uint32_t offset = 0;
            stride = 0;
            for (auto& element : elements)
            {
                element.offset = offset;
                offset += element.size;
                stride += element.size;
            }
        }

    private:
        std::vector<BufferElement> elements;
        uint32_t stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;

        virtual void set_data(const void* data, uint32_t size) = 0;

        virtual BufferLayout& get_layout() = 0;
        virtual void set_layout(const BufferLayout& layout) = 0;

        static std::shared_ptr<VertexBuffer> create(uint32_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;

        virtual uint32_t get_count() const = 0;

        static std::shared_ptr<IndexBuffer> create(const uint32_t* indices, uint32_t count);
    };

    struct IndirectCommand
    {
        uint32_t count;        // Number of elements to be drawn per instance
        uint32_t inst_count;   // Number of instances
        uint32_t first_idx;    // Offset of mesh in index buffer
        int32_t base_vertex;   // Offset of mesh in vertex buffer
        uint32_t base_inst;    // First instanced model index
    };

    class IndirectBuffer
    {
    public:
        virtual ~IndirectBuffer() = default;

        virtual void bind() const = 0;

        virtual uint32_t get_count() const = 0;

        static std::unique_ptr<IndirectBuffer> create(const std::vector<IndirectCommand>& commands);
    };

    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void bind() const = 0;

        virtual void add_vertex_buffer(std::shared_ptr<VertexBuffer> buf) = 0;
        virtual void set_index_buffer(std::shared_ptr<IndexBuffer> buf) = 0;

        virtual uint32_t get_index_count() const = 0;

        static std::shared_ptr<VertexArray> create();
    };
}
