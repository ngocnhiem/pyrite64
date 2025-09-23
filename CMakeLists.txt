cmake_minimum_required(VERSION 4.0)
project(p64editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(SDL_SHARED OFF)
## set(SDL_STATIC ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

#make an imgui folder and put all of these files in the imgui folder
set(IMGUI_SRC
        vendored/imgui/backends/imgui_impl_sdl3.cpp
        vendored/imgui/backends/imgui_impl_sdl3.h
        vendored/imgui/backends/imgui_impl_sdlgpu3.cpp
        vendored/imgui/backends/imgui_impl_sdlgpu3.h
        vendored/imgui/imgui.h
        vendored/imgui/imgui.cpp
        vendored/imgui/imgui_draw.cpp
        vendored/imgui/imgui_tables.cpp
        vendored/imgui/imgui_widgets.cpp
)

# Create your game executable target as usual
add_executable(p64editor src/main.cpp ${IMGUI_SRC}
        src/editor/editorScene.h
        src/editor/editorScene.cpp
)

target_include_directories(p64editor PRIVATE vendored/imgui)

# Link to the actual SDL3 library.
target_link_libraries(p64editor PRIVATE SDL3::SDL3)
