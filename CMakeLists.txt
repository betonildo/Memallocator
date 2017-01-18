cmake_minimum_required (VERSION 2.6)
project (Memallocator)

# custom flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 ")

#Add "include" folder to include into final build system
# Include Unity definitions
include_directories(${UnityDefinitions_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}/include")

# include translation units
file(GLOB TRANSLATION_UNITS src/*.cpp)
file(GLOB INCLUDE_UNITS include/*.h)

# creates a executable
set(TRANSLATION_UNITS "${TRANSLATION_UNITS}" main.cpp)
add_executable(Memallocator "${TRANSLATION_UNITS}" "${INCLUDE_UNITS}")

# Include transiently it's own dependencies
set(${PROJECT_NAME}_INCLUDE_DIRS 
    ${PROJECT_SOURCE_DIR}/include 
    ${UnityDefinitions_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)