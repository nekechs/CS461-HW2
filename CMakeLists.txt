cmake_minimum_required(VERSION 3.15)

add_compile_options(-fsanitize=address,undefined -Wall)
add_link_options(-fsanitize=address,undefined -Wall)

project(bruh-moment)
add_compile_definitions(OUTPUT_DIR="${PROJECT_SOURCE_DIR}/output")
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/output")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_subdirectory(src)


# add_subdirectory()