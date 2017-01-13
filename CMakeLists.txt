cmake_minimum_required (VERSION 2.6)
project (Memallocator)

# custom flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

#Add "include" folder to include into final build system
include_directories("${PROJECT_SOURCE_DIR}/include")

# include translation units
set(TRANSLATION_UNITS src/Memallocator.cpp)
set(INCLUDE_UNITS include/Memallocator.h include/Environment.h)

add_executable(Memallocator main.cpp "${TRANSLATION_UNITS}" "${INCLUDE_UNITS}")

