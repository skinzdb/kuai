#pragma once

namespace kuai {
    class RendererAPI
    {
    public:
        enum class API {
            None = 0,
            OpenGL = 1,
            Vulkan = 2
        };

    	virtual ~RendererAPI() = default;

        virtual void init() = 0;
        virtual void clear() = 0;

        static API getAPI() { return api; }
        static std::unique_ptr<RendererAPI> create();
    private:
        static API api;
    };
}
