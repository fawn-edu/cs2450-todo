cmake_minimum_required(VERSION 3.30)

project(
	todo
	VERSION 3.0.0
	DESCRIPTION "CS 2450 Task Manager"
	LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# https://github.com/fltk/fltk/blob/master/README.CMake.txt
FetchContent_Declare(FLTK
	GIT_REPOSITORY  https://github.com/fltk/fltk
	GIT_TAG         release-1.4.5
	GIT_SHALLOW     TRUE
	FIND_PACKAGE_ARGS
		NAMES fltk
		CONFIG
)

message(STATUS "Download and build FLTK if necessary, please wait...")
set(FLTK_BUILD_FLTK_OPTIONS OFF)
set(FLTK_BUILD_FLUID OFF)
FetchContent_MakeAvailable(FLTK)
message(STATUS "Download and build FLTK - done.")

add_executable(${PROJECT_NAME}
	main.cpp
	task.cpp
	task_controller.cpp
	task_view.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${PROJECT_NAME} PRIVATE fltk::fltk)
