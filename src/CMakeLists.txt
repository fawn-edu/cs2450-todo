cmake_minimum_required(VERSION 3.10)
project(
	todo
	VERSION 2.0.0
	DESCRIPTION "CS 2450 Task Manager"
	HOMEPAGE_URL "https://github.com/fawn-edu/cs2450-taskmanager"
	LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_STATIC ON CACHE INTERNAL "")
add_subdirectory(vendor/SDL)

add_executable(${PROJECT_NAME}
	# vendor/imgui/examples/example_sdl3_sdlgpu3/main.cpp
	main.cpp task.cpp task_controller2.cpp
	vendor/imgui/imgui.cpp vendor/imgui/imgui_demo.cpp
	vendor/imgui/imgui_draw.cpp vendor/imgui/imgui_tables.cpp
	vendor/imgui/imgui_widgets.cpp
	vendor/imgui/backends/imgui_impl_sdl3.cpp
	vendor/imgui/backends/imgui_impl_sdlgpu3.cpp
	vendor/imgui/misc/cpp/imgui_stdlib.cpp
)

# add_executable(${PROJECT_NAME}
# 	main.cpp task.cpp task_controller.cpp task_view.cpp
# )

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
	vendor/imgui
	vendor/imgui/backends
)
target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)

# TODO: Convert ad-hoc tests from Makefile, use CTest
