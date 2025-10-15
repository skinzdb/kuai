#pragma once

#include "rekuai/Renderer/Framebuffer.h"

namespace kuai {
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(uint32_t width, uint32_t height, uint32_t samples, uint32_t attachments);
        ~OpenGLFramebuffer();

        uint32_t get_depth_attachment();
        const std::vector<uint32_t>& get_col_attachments();

        void bind();
        void unbind();

        void resize(uint32_t width, uint32_t height);

    private:
        void reset();

        void attach_col_texture(uint32_t index);
        void attach_depth_texture();

        unsigned int get_texture_target(bool multisampling);

    private:
        uint32_t id = 0;

        std::vector<uint32_t> col_attachments;
        uint32_t depth_attachments = 0;

        FramebufferProps props;
    };
}
