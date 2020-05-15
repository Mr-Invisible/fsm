cmake_minimum_required(VERSION 3.0)

project(example)

include_directories(
    ../../
)

add_executable(
    example.out

    ${CMAKE_SOURCE_DIR}/../fsm.c
    example.c
)

target_compile_options(
    example.out
    PUBLIC
    -g
)

add_custom_target(
    launch
    COMMAND ./example.out
    VERBATIM
    DEPENDS example.out
)