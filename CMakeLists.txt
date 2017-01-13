cmake_minimum_required (VERSION 2.6)
project (Memallocator)

#Add "include" folder to include into final build system
include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(Memallocator main.cpp src/Memallocator.cpp)

