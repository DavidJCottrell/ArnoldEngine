# =============================================================================
# Project Configuration
# =============================================================================
cmake_minimum_required(VERSION 3.12)

project(ArnoldEngine
        VERSION 0.0.1
        LANGUAGES C CXX
        DESCRIPTION "ArnoldEngine"
)

# =============================================================================
# Compiler and Platform Settings
# =============================================================================
# Global C++ settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC-specific settings
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    add_compile_options(/W0)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# =============================================================================
# Build Options and Paths
# =============================================================================
option(BUILD_TESTS "Build test suite" ON)

# Define important paths
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(AE_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

# =============================================================================
# External Dependencies
# =============================================================================
# Third-party libraries with CMake support
if (NOT TARGET gtest)
    add_subdirectory("${EXTERNAL_DIR}/googletest")
endif()

if (NOT TARGET glfw)
    add_subdirectory("${EXTERNAL_DIR}/glfw")
endif()

if (NOT TARGET glm)
    add_subdirectory("${EXTERNAL_DIR}/glm")
endif()

if (NOT TARGET spdlog)
    add_subdirectory("${EXTERNAL_DIR}/spdlog")
endif()

if (NOT TARGET ccm)
    add_subdirectory("${EXTERNAL_DIR}/ccm")
endif()

# Header-only libraries
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE "${EXTERNAL_DIR}/entt")

# GLAD setup
add_library(glad STATIC "${EXTERNAL_DIR}/glad/src/glad.c")
target_include_directories(glad PUBLIC "${EXTERNAL_DIR}/glad/include")

# ImGui setup
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
# Platform-Specific Window and Input Implementations
# =============================================================================
set(AE_PLATFORM_SOURCES "")
if (APPLE)
    list(APPEND AE_PLATFORM_SOURCES
            "${AE_SOURCE_PATH}/Platform/Mac/MacWindow.cpp"
            "${AE_SOURCE_PATH}/Platform/Mac/MacInput.cpp"
    )
    find_package(OpenGL REQUIRED)
elseif (WIN32)
    list(APPEND AE_PLATFORM_SOURCES
            "${AE_SOURCE_PATH}/Platform/Windows/WindowsWindow.cpp"
            "${AE_SOURCE_PATH}/Platform/Windows/WindowsInput.cpp"
    )
elseif (UNIX AND NOT APPLE)
    list(APPEND AE_PLATFORM_SOURCES
            "${AE_SOURCE_PATH}/Platform/Linux/LinuxWindow.cpp"
            "${AE_SOURCE_PATH}/Platform/Linux/LinuxInput.cpp"
    )
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

# =============================================================================
# Main Library Target
# =============================================================================
add_library(${PROJECT_NAME} STATIC
        "${AE_SOURCE_PATH}/Core/Application.cpp"
        "${AE_SOURCE_PATH}/Core/Log.cpp"
        "${AE_SOURCE_PATH}/Core/Window.cpp"
        "${AE_SOURCE_PATH}/Core/Layer.cpp"
        "${AE_SOURCE_PATH}/Core/LayerStack.cpp"
        "${AE_SOURCE_PATH}/Graphics/UI/ImGuiLayer.cpp"
        "${AE_SOURCE_PATH}/Graphics/UI/ImGuiBuild.cpp"
        ${AE_PLATFORM_SOURCES}
)

# Precompiled headers
target_precompile_headers(${PROJECT_NAME}
        PRIVATE src/aepch.h
)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
        PUBLIC glfw
        PUBLIC glad
        PUBLIC entt
        PUBLIC imgui
        PUBLIC spdlog::spdlog
        PUBLIC ccm
        PUBLIC glm::glm
)

# Platform-specific links
if (APPLE)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE "-framework OpenGL"
    )
endif ()

# =============================================================================
# Tests Configuration
# =============================================================================
if (BUILD_TESTS)
    enable_testing()

    add_executable(${PROJECT_NAME}_tests
            tests/LayerStackTests.cpp
    )

    target_include_directories(${PROJECT_NAME}_tests PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )

    if(MSVC)
        set_property(TARGET ${PROJECT_NAME}_tests PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        target_compile_options(${PROJECT_NAME}_tests PRIVATE /W0)
    endif()

    target_link_libraries(${PROJECT_NAME}_tests
            PRIVATE GTest::gtest_main
            PRIVATE ${PROJECT_NAME}
    )

    add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
endif()

# =============================================================================
# Installation Rules
# =============================================================================
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)