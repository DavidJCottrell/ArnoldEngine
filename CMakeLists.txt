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
# ArnoldEngine
# ============================
add_library(${PROJECT_NAME}
    src/Application.cpp
    src/Window.cpp
    src/Log.cpp
)

target_precompile_headers(ArnoldEngine PRIVATE src/aepch.h)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC glfw
    PUBLIC glad
    PUBLIC entt
    PUBLIC imgui
    PUBLIC spdlog::spdlog
)