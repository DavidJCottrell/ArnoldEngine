cmake_minimum_required(VERSION 3.12)
project(ArnoldEngine VERSION 0.0.1 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# External dependencies base directory
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

# ============================
# EnTT (Header-only library)
# ============================
set(ENTT_PATH ${EXTERNAL_DIR}/entt)
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE ${ENTT_PATH})

# ============================
# GLFW
# ============================
set(GLFW_PATH ${EXTERNAL_DIR}/glfw)
add_subdirectory(${GLFW_PATH})

# ============================
# GLM
# ============================
set(GLM_PATH ${EXTERNAL_DIR}/glm)
add_subdirectory(${GLM_PATH})
target_include_directories(glm PUBLIC ${GLM_PATH}/glm)

# ============================
# GLAD
# ============================
set(GLAD_PATH ${EXTERNAL_DIR}/glad)
add_library(glad STATIC ${GLAD_PATH}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_PATH}/include)

# ============================
# ImGui
# ============================
set(IMGUI_PATH ${EXTERNAL_DIR}/imgui)
set(IMGUI_BACKENDS ${IMGUI_PATH}/backends)

set(IMGUI_SOURCES
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp
        ${IMGUI_BACKENDS}/imgui_impl_glfw.cpp
        ${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
        ${IMGUI_PATH}
        ${IMGUI_BACKENDS}
)
target_link_libraries(imgui PUBLIC glfw glad)

# ============================
# SPDLOG
# ============================
set(SPDLOG_PATH ${EXTERNAL_DIR}/spdlog)
add_subdirectory(${SPDLOG_PATH})

# ============================
# CCM
# ============================
set(CCM_PATH ${EXTERNAL_DIR}/ccm)
add_subdirectory(${CCM_PATH})

# ============================
# ArnoldEngine
# ============================
add_library(${PROJECT_NAME} SHARED
        src/Core/Application.cpp
        src/Core/Log.cpp
        src/Core/Window.cpp
        src/Core/Layer.cpp
        src/Core/LayerStack.cpp
        src/Graphics/UI/ImGuiLayer.cpp
        src/Graphics/UI/ImGuiBuild.cpp
        src/Platform/Mac/MacWindow.cpp
        src/Platform/Mac/MacInput.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ARNOLDENGINE_BUILD_DLL)

target_precompile_headers(ArnoldEngine PRIVATE src/aepch.h)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

target_link_options(${PROJECT_NAME} PRIVATE LINKER:-no_warn_duplicate_libraries)

if (APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
endif ()

target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw
        PRIVATE glad
        PRIVATE entt
        PUBLIC imgui
        PUBLIC spdlog::spdlog
        PRIVATE ccm
        PRIVATE glm::glm
)
