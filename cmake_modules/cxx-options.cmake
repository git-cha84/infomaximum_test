cmake_minimum_required(VERSION 3.10)

enable_language(C)
enable_language(CXX)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)


# Set C++ compiler common options
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options( -Wall )

set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -Wall")