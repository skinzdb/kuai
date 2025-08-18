#pragma once

namespace kuai {
    const uint32_t MAX_FRAMEBUFFER_SIZE = 3840;

    struct FramebufferProps
    {
        uint32_t width = 0, height = 0;
        uint32_t samples = 1;
        uint32_t attachments = 0;
    };

    /** \class Framebuffer
	*	\brief Holds a texture that gets rendered to.
	*/
    class Framebuffer
    {
    public:
        Framebuffer(uint32_t width, uint32_t height, uint32_t samples, uint32_t attachments);
        ~Framebuffer();

        const uint32_t get_depth_attachment();
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
