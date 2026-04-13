#pragma once

#include "Arnold/Core/Application.h"
#include "Arnold/Core/Layer.h"
#include "Arnold/Core/Log.h"
#include "Arnold/Graphics/UI/ImGuiLayer.h"

#include "Arnold/Core/Timestep.h"

// --- Input ---
#include "Arnold/Core/Input.h"
#include "Arnold/Core/KeyCodes.h"
#include "Arnold/Core/MouseButtonCodes.h"

// --- Rendering ---
#include "Arnold/Graphics/Renderer/Renderer.h"
#include "Arnold/Graphics/Renderer/RenderCommand.h"

#include "Arnold/Graphics/Renderer/Buffer.h"
#include "Arnold/Graphics/Renderer/Shader.h"
#include "Arnold/Graphics/Renderer/VertexArray.h"

#include "Arnold/Graphics/Renderer/OrthographicCamera.h"
#include "Arnold/Graphics/Renderer/Camera.h"
#include "Arnold/Graphics/Renderer/PerspectiveCamera.h"
#include "Arnold/Graphics/Renderer/FpsCameraController.h"
#include "Arnold/Graphics/Renderer/Mesh.h"
#include "Arnold/Graphics/Renderer/Texture.h"
#include "Arnold/Graphics/Renderer/Material.h"
#include "Arnold/Graphics/Renderer/TextureAtlas.h"

// --- World ---
#include "Arnold/World/Chunk.h"
#include "Arnold/World/World.h"
#include "Arnold/World/Raycast.h"

// --- High-level Facade ---
#include "Arnold/Facade/VoxelSceneConfig.h"
#include "Arnold/Facade/VoxelScene.h"
#include "Arnold/Facade/VoxelLayer.h"

// --- Entry Point ---
#ifdef AE_MAIN
#include "Arnold/Core/EntryPoint.h"
#endif
