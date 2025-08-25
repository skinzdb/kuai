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
        virtual ~Framebuffer() = default;

        virtual uint32_t get_depth_attachment();
        virtual const std::vector<uint32_t>& get_col_attachments();

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void resize(uint32_t width, uint32_t height) = 0;

        static std::shared_ptr<Framebuffer> create(uint32_t width, uint32_t height, uint32_t samples, uint32_t attachments);
    };
}
