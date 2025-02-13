# Arnold Engine {#mainpage}

## Overview

Arnold Engine is a cross-platform game engine built in C++. It provides a robust foundation for game development with
features including:

- Cross-platform support (Windows, macOS, Linux)
- Event system for handling input and window events
- Layer-based architecture for organized game logic
- ImGui integration for debugging and tools
- Modern OpenGL rendering
- Built-in logging system

## Prerequisites

- CMake (3.12 or higher)
- C++20 compatible compiler
- Git

When building on Linux, you will need to have the following development libraries installed on you system:

- Wayland
- pkg-config
- X11
- OpenGL

To install all of these on Ubuntu/Debian systems, run:

```bash
sudo apt-get install libwayland-dev wayland-protocols extra-cmake-modules libxkbcommon-dev pkg-config libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev mesa-common-dev libgl1-mesa-dev
```

For Fedora:

```bash
sudo dnf install wayland-devel wayland-protocols-devel extra-cmake-modules libxkbcommon-devel pkgconfig libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel mesa-libGL-devel
```

Arch Linux:

```bash
sudo pacman -S wayland wayland-protocols extra-cmake-modules libxkbcommon pkg-config libx11 libxrandr libxinerama libxcursor libxi mesa
```

## Getting Started

### Clone the Repository

Clone the repository along with all of the submodules into an `/external` directory within your project:

```bash
git clone --recurse-submodules https://github.com/DavidJCottrell/ArnoldEngine.git /external/ArnoldEngine
cd /external/ArnoldEngine
```

If you already cloned the repository without submodules, you can initialize them with:

```bash
git submodule update --init --recursive
```

### Building from Source

1. Create a build directory:

```bash
mkdir build && cd build
```

2. Generate build files and compile:

```bash
cmake ..
cmake --build .
```

### Using Arnold Engine in Your Project

To use Arnold Engine in your own project, add it as a subdirectory in your CMake project:

```cmake
add_subdirectory({CMAKE_CURRENT_SOURCE_DIR}/external/ArnoldEngine)
...
target_link_libraries(YourGameTarget ArnoldEngine)
```

## Basic Example

Here's a minimal example of creating a game using Arnold Engine:

```cpp
#include <ArnoldEngine.h>

class ExampleGame : public AE::Core::Application {
public:
    ExampleGame() {
        // Initialize your game here
    }
};

AE::Core::Application* AE::Core::CreateApplication() {
    return new ExampleGame();
}
```

## Project Structure

- `src/Core/` - Core engine systems
- `src/Events/` - Event system implementation
- `src/Graphics/` - Rendering and UI systems
- `src/Platform/` - Platform-specific implementations for Windowing and Input
- `include/` - Public API headers
- `tests/` - Engine test suite

## Dependencies

Arnold Engine uses several external libraries (automatically handled via Git submodules):

- GLFW - Window creation and input
- Dear ImGui - Debug UI and tools
- Glad - OpenGL loading
- spdlog - Logging system
- GLM - Mathematics library
- GoogleTest - Testing framework
