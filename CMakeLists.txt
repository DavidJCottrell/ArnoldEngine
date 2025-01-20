cmake_minimum_required(VERSION 3.12)

# =============================================================================
# Project Configuration
# =============================================================================
project(ArnoldEngine
        VERSION 0.0.1
        LANGUAGES C CXX
        DESCRIPTION "ArnoldEngine"
)

# =============================================================================
# Global Settings
# =============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Important paths
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

# Build options
option(BUILD_TESTS "Build test suite" ON)

# =============================================================================
# Platform-Specific Settings
# =============================================================================
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    add_compile_options(/W0)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()

# =============================================================================
# External Dependencies
# =============================================================================
# Core dependencies
find_package(OpenGL REQUIRED)

# Third-party CMake libraries
set(THIRD_PARTY_LIBS
        googletest
        glfw
        glm
        spdlog
        ACM
)

foreach (lib ${THIRD_PARTY_LIBS})
    if (NOT TARGET ${lib})
        add_subdirectory("${EXTERNAL_DIR}/${lib}")
    endif ()
endforeach ()

# Header-only libraries
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE "${EXTERNAL_DIR}/entt")

# GLAD
add_library(glad STATIC "${EXTERNAL_DIR}/glad/src/glad.c")
target_include_directories(glad PUBLIC "${EXTERNAL_DIR}/glad/include")

# ImGui
set(IMGUI_PATH "${EXTERNAL_DIR}/imgui")
add_library(imgui STATIC
        "${IMGUI_PATH}/imgui.cpp"
        "${IMGUI_PATH}/imgui_demo.cpp"
        "${IMGUI_PATH}/imgui_draw.cpp"
        "${IMGUI_PATH}/imgui_tables.cpp"
        "${IMGUI_PATH}/imgui_widgets.cpp"
        "${IMGUI_PATH}/backends/imgui_impl_glfw.cpp"
        "${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui PUBLIC
        "${IMGUI_PATH}"
        "${IMGUI_PATH}/backends"
)
target_link_libraries(imgui PUBLIC glfw glad)

# =============================================================================
# Platform-Specific Source Files
# =============================================================================
set(RENDERER_SOURCES
        src/Platform/RenderingAPI/OpenGL/OpenGLContext.cpp
        src/Platform/RenderingAPI/OpenGL/OpenGLBuffer.cpp
        src/Platform/RenderingAPI/OpenGL/OpenGLVertexArray.cpp
        src/Platform/RenderingAPI/OpenGL/OpenGLRendererAPI.cpp
)

# Platform-specific files
if (APPLE)
    list(APPEND PLATFORM_SOURCES
            "src/Platform/Mac/MacWindow.cpp"
            "src/Platform/Mac/MacInput.cpp"
    )
elseif (WIN32)
    list(APPEND PLATFORM_SOURCES
            "src/Platform/Windows/WindowsWindow.cpp"
            "src/Platform/Windows/WindowsInput.cpp"
    )
elseif (UNIX AND NOT APPLE)
    list(APPEND PLATFORM_SOURCES
            "src/Platform/Linux/LinuxWindow.cpp"
            "src/Platform/Linux/LinuxInput.cpp"
    )
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

# =============================================================================
# Main Library Target
# =============================================================================
set(CORE_SOURCES
        "src/Arnold/Core/Application.cpp"
        "src/Arnold/Core/Log.cpp"
        "src/Arnold/Core/Window.cpp"
        "src/Arnold/Core/Layer.cpp"
        "src/Arnold/Core/LayerStack.cpp"
)

set(GRAPHICS_SOURCES
        "src/Arnold/Graphics/UI/ImGuiLayer.cpp"
        "src/Arnold/Graphics/UI/ImGuiBuild.cpp"
        "src/Arnold/Graphics/Renderer/Shader.cpp"
        "src/Arnold/Graphics/Renderer/Buffer.cpp"
        "src/Arnold/Graphics/Renderer/Renderer.cpp"
        "src/Arnold/Graphics/Renderer/VertexArray.cpp"
        "src/Arnold/Graphics/Renderer/RendererAPI.cpp"
        "src/Arnold/Graphics/Renderer/RendererAPI.h"
        "src/Arnold/Graphics/Renderer/RenderCommand.cpp"
        "src/Arnold/Graphics/Renderer/RenderCommand.h"
        src/Arnold/Graphics/Renderer/OrthographicCamera.cpp
        src/Arnold/Graphics/Renderer/OrthographicCamera.h
)

add_library(${PROJECT_NAME} STATIC
        ${CORE_SOURCES}
        ${GRAPHICS_SOURCES}
        ${RENDERER_SOURCES}
        ${PLATFORM_SOURCES}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/aepch.h)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        glad
        entt
        imgui
        spdlog::spdlog
        ACM
        glm::glm
)

# Platform-specific linking
if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
endif ()

# =============================================================================
# Tests Configuration
# =============================================================================
if (BUILD_TESTS)
    enable_testing()

    add_executable(${PROJECT_NAME}_tests
            tests/LayerStackTests.cpp
            tests/EventHandlerTests.cpp
    )

    target_include_directories(${PROJECT_NAME}_tests PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )

    if (MSVC)
        set_property(TARGET ${PROJECT_NAME}_tests PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        target_compile_options(${PROJECT_NAME}_tests PRIVATE /W0)
    endif ()

    target_link_libraries(${PROJECT_NAME}_tests PRIVATE
            GTest::gtest_main
            ${PROJECT_NAME}
    )

    add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
endif ()

# =============================================================================
# Installation Configuration
# =============================================================================
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
