add_library(
    libFsm
    fsm.c
)

target_include_directories(
    libFsm
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
