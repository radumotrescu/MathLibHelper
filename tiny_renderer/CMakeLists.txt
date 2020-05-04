CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
cmake_policy(SET CMP0091 NEW)
PROJECT(TinyRenderer CXX)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE RENDERER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE RENDERER_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(files_all ${RENDERER_SOURCE} ${RENDERER_HEADER})

add_executable(TinyRenderer ${files_all})

set_property (TARGET TinyRenderer
  PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17)

set_property (TARGET TinyRenderer
  PROPERTY 
    # Enable /MD
    MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
)

target_link_libraries(TinyRenderer MathLibHelper ${CONAN_LIBS})
