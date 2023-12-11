cmake_minimum_required(VERSION 3.10)

find_package(Threads)
link_libraries(Threads::Threads)


if(UNIX)
  set(NEED_LIBS "pthread")
endif()
