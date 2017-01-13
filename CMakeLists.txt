cmake_minimum_required (VERSION 2.6)
project (Memallocator)
# The version number.
set (Memallocator_VERSION_MAJOR 1)
set (Memallocator_VERSION_MINOR 0)

# custom flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")  ## Strip binary

#Add "include" folder to include into final build system
include_directories("${PROJECT_SOURCE_DIR}/include")

# include translation units
set(TRANSLATION_UNITS 
    src/Memallocator.cpp)
    
set(INCLUDE_UNITS 
    include/Memallocator.h 
    include/Environment.h 
    include/EnvTypes.h)

add_executable(Memallocator main.cpp "${TRANSLATION_UNITS}" "${INCLUDE_UNITS}")

