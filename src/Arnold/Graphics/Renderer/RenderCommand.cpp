#include "aepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace AE::Graphics::Renderer
{
    // TODO: set dynamically
    RendererAPI* RenderCommand::s_RendererAPI = new Platform::OpenGL::OpenGLRendererAPI;
}

