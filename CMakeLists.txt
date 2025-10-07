cmake_minimum_required(VERSION 3.5)
project(simpleServer C)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES simpleServer.c)
add_executable(simpleServer ${SOURCE_FILES})

# recommended compiler flags for portability
if(MSVC)
  target_compile_options(simpleServer PRIVATE /W4)
else()
  target_compile_options(simpleServer PRIVATE -Wall -Wextra -pedantic)
endif()
