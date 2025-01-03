#pragma once

#include "Core/Application.h"
#include "Core/Layer.h"
#include "Graphics/UI/ImGuiLayer.h"
#include "Core/Log.h"

// --- Input ---
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseButtonCodes.h"

// Only include entry point in the main application file
#ifdef AE_MAIN
#include "Core/EntryPoint.h"
#endif
