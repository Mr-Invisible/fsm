cmake_minimum_required(VERSION 3.0)

project(example)

add_subdirectory(
    ../
    ./libFsm
)

add_executable(
    example.out
    example.c
)

target_link_libraries(
    example.out
    LINK_PUBLIC
    libFsm
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