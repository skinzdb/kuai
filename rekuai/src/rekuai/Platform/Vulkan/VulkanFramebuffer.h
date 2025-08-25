#pragma once

#include "rekuai/Renderer/Framebuffer.h"

namespace kuai {

    class VulkanFrameBuffer : public Framebuffer
    {
    public:
        virtual uint32_t get_depth_attachment() override;
        virtual const std::vector<uint32_t>& get_col_attachments() override;

        virtual void bind() override;
        virtual void unbind() override;

        virtual void resize(uint32_t width, uint32_t height) override;
    };
}
