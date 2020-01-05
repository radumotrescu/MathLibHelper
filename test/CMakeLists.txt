CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(MathLibHelper-test CXX)

file(GLOB_RECURSE TEST_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(files_all ${TEST_SOURCE})


add_executable(MathLibHelper_test ${files_all})
target_link_libraries(MathLibHelper_test MathLibHelper ${CONAN_LIBS})

# Include Encryptor test #######################################################
ENABLE_TESTING()
ADD_TEST(NAME test
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
         COMMAND ${PROJECT_NAME})