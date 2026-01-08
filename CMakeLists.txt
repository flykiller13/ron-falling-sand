cmake_minimum_required(VERSION 3.28)
project(FallingSand)

set(CMAKE_CXX_STANDARD 20)

#Find Dependencies
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Create ImGui library
add_library(imgui STATIC
        include/imgui-master/imgui.cpp
        include/imgui-master/imgui_draw.cpp
        include/imgui-master/imgui_tables.cpp
        include/imgui-master/imgui_widgets.cpp
        include/imgui-master/backends/imgui_impl_glfw.cpp
        include/imgui-master/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        include/imgui-master
        include/imgui-master/backends
)

target_link_libraries(imgui PUBLIC OpenGL::GL ${GLFW_LIBRARIES})

add_executable(FallingSand
        src/glad.c
        src/renderer/shader.cpp
        src/renderer/renderer.cpp
        src/simulation/grid.cpp
        src/simulation/simulation.cpp
        src/ui.cpp
        src/input.cpp
        src/app.cpp
        src/main.cpp
)

# Include directories
target_include_directories(FallingSand PUBLIC
        include
)

# Link Libraries
target_link_libraries(FallingSand PRIVATE OpenGL::GL ${GLFW_LIBRARIES} imgui)