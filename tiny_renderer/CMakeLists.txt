CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(TinyRenderer CXX)

file(GLOB_RECURSE RENDERER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE RENDERER_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(files_all ${RENDERER_SOURCE} ${RENDERER_HEADER})

add_executable(TinyRenderer ${files_all})
target_link_libraries(TinyRenderer MathLibHelper ${CONAN_LIBS})
