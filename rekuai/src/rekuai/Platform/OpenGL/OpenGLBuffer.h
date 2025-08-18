#pragma once

#include "rekuai/Core/Core.h"
#include "rekuai/Renderer/Buffer.h"

namespace kuai {

    class OpenGLBuffer : public VertexBuffer
    {
    public:
        OpenGLBuffer(uint32_t size);
        ~OpenGLBuffer();

        void bind() const override;
        void unbind() const override;

        void set_data(const void* data, uint32_t size);
        void reset(const void* data, uint32_t size, DrawHint drawHint = DrawHint::STATIC);

        BufferLayout& get_layout() { return layout; }
        void set_layout(const BufferLayout& layout) { this->layout = layout; }

    private:
        uint32_t buf_id;
        BufferLayout layout;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer(uint32_t* indices, uint32_t count);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;

        uint32_t get_count() const { return count; }

    private:
        uint32_t buf_id;
        uint32_t count;
    };

    struct IndirectCommand
    {
        uint32_t count;        // Number of elements to be drawn per instance
        uint32_t inst_count;   // Number of instances
        uint32_t first_idx;    // Offset of mesh in index buffer
        int32_t base_vertex;  // Offset of mesh in vertex buffer
        uint32_t base_inst;    // First instanced model index
    };

    class IndirectBuffer
    {
    public:
        IndirectBuffer(const std::vector<IndirectCommand>& commands);
        ~IndirectBuffer();

        void bind() const;
        void unbind() const;

        uint32_t get_count() const { return count; }

    private:
        uint32_t buf_id;
        uint32_t count;
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void bind() const;
        void unbind() const;

        void add_vertex_buffer(std::unique_ptr<VertexBuffer> buf);

        void set_index_buffer(std::unique_ptr<IndexBuffer> buf);

    private:
        std::vector<std::unique_ptr<VertexBuffer>> vertex_bufs;
        std::unique_ptr<IndexBuffer> index_buf;
        uint32_t vao_id;
        uint32_t index = 0;
    };
}
