#include "aepch.h"
#include "RenderCommand.h"

#include "Platform/RenderingAPI/OpenGL/OpenGLRendererAPI.h"

namespace AE::Graphics::Renderer
{
    // TODO: set dynamically
    RendererAPI* RenderCommand::s_RendererAPI = new Platform::OpenGL::OpenGLRendererAPI;
}

