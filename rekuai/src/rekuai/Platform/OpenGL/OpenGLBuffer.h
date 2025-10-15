#pragma once

#include "rekuai/Renderer/Buffer.h"

namespace kuai {

    class OpenGLBuffer : public VertexBuffer
    {
    public:
        OpenGLBuffer(uint32_t size);
        ~OpenGLBuffer();

        void bind() const override;

        void set_data(const void* data, uint32_t size) override;
        void reset(const void* data, uint32_t size);

        BufferLayout& get_layout() override { return layout; }
        void set_layout(const BufferLayout& layout) override { this->layout = layout; }

    private:
        uint32_t buf_id;
        BufferLayout layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer();

        void bind() const override;

        uint32_t get_count() const override { return count; }

    private:
        uint32_t buf_id;
        uint32_t count;
    };

    class OpenGLIndirectBuffer : public IndirectBuffer
    {
    public:
        OpenGLIndirectBuffer(const std::vector<IndirectCommand>& commands);
        ~OpenGLIndirectBuffer();

        virtual void bind() const override;

        virtual uint32_t get_count() const override { return count; }

    private:
        uint32_t buf_id;
        uint32_t count;
    };

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void bind() const override;

        void add_vertex_buffer(std::shared_ptr<VertexBuffer> buf) override;
        void set_index_buffer(std::shared_ptr<IndexBuffer> buf) override;

        uint32_t get_index_count() const override { return index_buf->get_count(); }

    private:
        std::vector<std::shared_ptr<VertexBuffer>> vertex_bufs;
        std::shared_ptr<IndexBuffer> index_buf;

        uint32_t vao_id;
        uint32_t index = 0;
    };
}
