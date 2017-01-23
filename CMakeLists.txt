cmake_minimum_required (VERSION 2.6)
project (Memallocator)

# custom flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 ")

#Add "include" folder to include into final build system
# Include Unity definitions
include_directories(${UnityDefinitions_INCLUDE_DIRS})
include_directories(${LTL_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}/include")

# include translation units
file(GLOB TRANSLATION_UNITS src/*.cpp)
file(GLOB INCLUDE_UNITS include/*.h)

# creates a executable
set(ALL_LOCAL_SOURCES 
    ${TRANSLATION_UNITS}
    ${UnitDefinitions_INCLUDE_UNITS}
    ${LTL_INCLUDE_UNITS} main.cpp)
    
add_executable(Memallocator "${ALL_LOCAL_SOURCES}" "${INCLUDE_UNITS}")

# Include transiently it's own dependencies
set(${PROJECT_NAME}_INCLUDE_DIRS 
    ${PROJECT_SOURCE_DIR}/include 
    ${UnityDefinitions_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

# Export Translation units
set(${PROJECT_NAME}_TRANSLATION_UNITS
    ${TRANSLATION_UNITS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(${PROJECT_NAME}_INCLUDE_UNITS
    ${INCLUDE_UNITS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)