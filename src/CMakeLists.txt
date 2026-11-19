cmake_minimum_required(VERSION 3.30)

project(
	todo
	VERSION 5.1.0
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
	$<$<CXX_COMPILER_ID:CLANG,GCC>:-Wall -Wextra -pedantic -Wno-missing-field-initializers>
	$<IF:$<AND:$<CXX_COMPILER_ID:Clang,GNU>,$<CONFIG:Debug>>,--coverage,>
)
target_link_options(${PROJECT_NAME} PRIVATE
	$<IF:$<AND:$<CXX_COMPILER_ID:Clang,GNU>,$<CONFIG:Debug>>,--coverage,>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(${PROJECT_NAME} PRIVATE fltk::fltk)

if(BUILD_TESTING)
	include(CTest)

	add_executable(test_task
		task.cpp
		task_controller.cpp
		test/test_task.cpp
	)
	target_compile_options(test_task PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/W4>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Wno-missing-field-initializers>
		$<IF:$<AND:$<CXX_COMPILER_ID:Clang,GNU>,$<CONFIG:Debug>>,--coverage,>
	)
	target_link_options(test_task PRIVATE
		$<IF:$<AND:$<CXX_COMPILER_ID:Clang,GNU>,$<CONFIG:Debug>>,--coverage,>
	)
	target_compile_definitions(test_task PRIVATE _CRT_SECURE_NO_WARNINGS)
	add_test(NAME "task" COMMAND test_task)

	add_executable(test_controller
		task.cpp
		task_controller.cpp
		test/test_controller.cpp
	)
	target_compile_options(test_controller PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/W4>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Wno-missing-field-initializers>
		$<IF:$<AND:$<CXX_COMPILER_ID:Clang,GNU>,$<CONFIG:Debug>>,--coverage,>
	)
	target_link_options(test_controller PRIVATE
		$<IF:$<AND:$<CXX_COMPILER_ID:Clang,GNU>,$<CONFIG:Debug>>,--coverage,>
	)
	target_compile_definitions(test_controller PRIVATE _CRT_SECURE_NO_WARNINGS)
	add_test(NAME "controller" COMMAND test_controller)

	add_executable(test_command
		task.cpp
		task_controller.cpp
		test/test_command.cpp
	)
	target_compile_options(test_command PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/W4>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Wno-missing-field-initializers>
		$<IF:$<AND:$<CXX_COMPILER_ID:Clang,GNU>,$<CONFIG:Debug>>,--coverage,>
	)
	target_link_options(test_command PRIVATE
		$<IF:$<AND:$<CXX_COMPILER_ID:Clang,GNU>,$<CONFIG:Debug>>,--coverage,>
	)
	target_compile_definitions(test_command PRIVATE _CRT_SECURE_NO_WARNINGS)
	add_test(NAME "command" COMMAND test_command)
endif()
