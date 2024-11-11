cmake_minimum_required(VERSION 3.26)
project(Project4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_subdirectory(libs/glfw)

# include the Directories duh
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include
)

#  Making a place for all of the source files
set(SOURCES
        src/main.cpp
        src/TravelPlanner.cpp
        src/GeneticAlgorithmWOC.cpp
        src/GuiHandaler.cpp
        # ImGui sources
        libs/imgui/imgui.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_demo.cpp
        libs/imgui/backends/imgui_impl_glfw.cpp
        libs/imgui/backends/imgui_impl_opengl3.cpp
        # ImPlot sources
        libs/implot/implot.cpp
        libs/implot/implot_items.cpp
        libs/implot/implot_demo.cpp
)

# Find the package
find_package(OpenGL REQUIRED)

# Add your executables for the project
add_executable(TSPGeneticAlgorithm ${SOURCES})

# Link libraries
target_link_libraries(TSPGeneticAlgorithm PRIVATE glfw OpenGL::GL)