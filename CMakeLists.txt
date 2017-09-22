cmake_minimum_required(VERSION 3.1)

project(Memallocator)

set (CMAKE_CXX_STANDARD 11)

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES_CPP "src/*.cpp")
# FOR PRE-COMPILED HEADERS
#file(GLOB_RECURSE Foo_HEADERS "src/*.h")
file(GLOB_RECURSE TEST_SOURCES_CPP "test/*.cpp")


add_library(Memallocator ${SOURCES_CPP})

add_executable(TestMemallocator ${TEST_SOURCES_CPP})
include_directories("${PROJECT_SOURCE_DIR}/test")
target_link_libraries(Memallocator)
