cmake_minimum_required(VERSION 2.8.12)
project(MathLibHelper)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(mathlib test.cpp)
target_link_libraries(mathlib ${CONAN_LIBS})