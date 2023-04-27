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
    class Framebuffer // TODO: rename as render texture?
    {
    public:
        Framebuffer(uint32_t width, uint32_t height, uint32_t samples, uint32_t attachments);
        ~Framebuffer();

        const uint32_t getDepthAttachment();
        const std::vector<uint32_t>& getColAttachments();

        void bind();
        void unbind();

        void resize(uint32_t width, uint32_t height);

    private:
        void reset();

        void attachColTexture(uint32_t index);
        void attachDepthTexture();

        unsigned int getTextureTarget(bool multisampling);

    private:
        uint32_t frameBufId = 0;

        std::vector<uint32_t> colAttachments;
        uint32_t depthAttachment = 0;

        FramebufferProps props;
    };
}