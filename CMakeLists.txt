cmake_minimum_required(VERSION 3.10)

project(test_task)

list(APPEND CMAKE_MODULE_PATH 
${CMAKE_SOURCE_DIR}/cmake_modules
)

include(enviroment)

add_subdirectory(lib)
add_subdirectory(src)

#add_clang_tidy_check_target()
