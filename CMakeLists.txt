cmake_minimum_required(VERSION 3.16)
project(yardiff VERSION 0.1 DESCRIPTION "Yet another rdiff implementation" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(lib)
add_subdirectory(tests)

add_executable(yardiff main.cpp)
target_link_libraries(yardiff PUBLIC yardifflib)
