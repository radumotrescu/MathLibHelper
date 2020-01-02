CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(MathLibHelper CXX)

set(CMAKE_CXX_STANDARD 14)

# Initialize Conan #############################################################
INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

include_directories(src)
FILE(GLOB MY_HEADERS "src/*.h")
FILE(GLOB MY_SOURCES "src/*.cpp")


# Build our project with the help of conan.
include_directories(${CONAN_INCLUDE_DIRS})
add_library(MathLibHelper STATIC ${MY_HEADERS} ${MY_SOURCES})
target_link_libraries(MathLibHelper ${CONAN_LIBS})
		
# Now enable our tests
enable_testing()
add_subdirectory(test)