#pragma once

namespace AE::Graphics::Renderer
{
    class Context
    {
    public:
        virtual ~Context() = default;
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}
