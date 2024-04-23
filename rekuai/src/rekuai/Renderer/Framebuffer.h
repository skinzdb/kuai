#pragma once

namespace kuai {
    const u32 MAX_FRAMEBUFFER_SIZE = 3840;

    struct FramebufferProps
    {
        u32 width = 0, height = 0;
        u32 samples = 1;
        u32 attachments = 0;
    };

    /** \class Framebuffer
	*	\brief Holds a texture that gets rendered to.
	*/
    class Framebuffer
    {
    public:
        Framebuffer(u32 width, u32 height, u32 samples, u32 attachments);
        ~Framebuffer();

        const u32 get_depth_attachment();
        const std::vector<u32>& get_col_attachments();

        void bind();
        void unbind();

        void resize(u32 width, u32 height);

    private:
        void reset();

        void attach_col_texture(u32 index);
        void attach_depth_texture();

        unsigned int get_texture_target(bool multisampling);

    private:
        u32 id = 0;

        std::vector<u32> col_attachments;
        u32 depth_attachments = 0;

        FramebufferProps props;
    };
}