#pragma once

namespace AE::Graphics::Renderer
{
    enum class RendererAPI
    {
        None = 0,
        OpenGL = 1,
        Vulkan = 2,
        DirectX = 3,
    };

    class Renderer
    {
    public:
        static RendererAPI GetAPI() { return s_RendererAPI; };

    private:
        static RendererAPI s_RendererAPI;
    };
}


