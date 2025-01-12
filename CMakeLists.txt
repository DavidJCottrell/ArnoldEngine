cmake_minimum_required(VERSION 3.12)
project(ArnoldEngine VERSION 0.0.1 LANGUAGES C CXX)

enable_testing()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# External dependencies base directory
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

# ============================
# GoogleTest
# ============================
set(GTEST_PATH ${EXTERNAL_DIR}/googletest)
add_subdirectory(${GTEST_PATH})

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
set(AE_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(AE_SOURCES
        ${AE_SOURCE_PATH}/Core/Application.cpp
        ${AE_SOURCE_PATH}/Core/Log.cpp
        ${AE_SOURCE_PATH}/Core/Window.cpp
        ${AE_SOURCE_PATH}/Core/Layer.cpp
        ${AE_SOURCE_PATH}/Core/LayerStack.cpp
        ${AE_SOURCE_PATH}/Graphics/UI/ImGuiLayer.cpp
        ${AE_SOURCE_PATH}/Graphics/UI/ImGuiBuild.cpp

)
if (APPLE)
    set(AE_PLATFORM_SOURCES
            ${AE_SOURCE_PATH}/Platform/Mac/MacWindow.cpp
            ${AE_SOURCE_PATH}/Platform/Mac/MacInput.cpp
    )
    message(STATUS "Configuring for macOS")
elseif (WIN32)
    set(AE_PLATFORM_SOURCES
            ${AE_SOURCE_PATH}/Platform/Windows/WindowsWindow.cpp
            ${AE_SOURCE_PATH}/Platform/Windows/WindowsInput.cpp
    )
    message(STATUS "Configuring for Windows")
elseif (UNIX AND NOT APPLE)
    set(AE_PLATFORM_SOURCES
            ${AE_SOURCE_PATH}/Platform/Linux/LinuxWindow.cpp
            ${AE_SOURCE_PATH}/Platform/Linux/LinuxInput.cpp
    )
    message(STATUS "Configuring for Linux")
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

add_library(${PROJECT_NAME} SHARED
        ${AE_SOURCES}
        ${AE_PLATFORM_SOURCES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ARNOLD_ENGINE_BUILD_DLL)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/aepch.h)

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

# ============================
# Tests
# ============================
add_executable(${PROJECT_NAME}_tests
        tests/LayerStackTests.cpp
)
target_link_libraries(${PROJECT_NAME}_tests
        PRIVATE GTest::gtest_main
        PRIVATE ${PROJECT_NAME}
)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)

target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw
        PRIVATE glad
        PRIVATE entt
        PUBLIC imgui
        PUBLIC spdlog::spdlog
        PRIVATE ccm
        PRIVATE glm::glm
)


