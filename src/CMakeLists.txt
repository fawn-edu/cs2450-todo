cmake_minimum_required(VERSION 3.30)

project(
    todo
    VERSION 2.0.0
    DESCRIPTION "CS 2450 Task Manager"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLTK CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    task.cpp
    task_controller.cpp
    task_view.cpp
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    fltk
)
