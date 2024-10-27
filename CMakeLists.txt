cmake_minimum_required(VERSION 3.15)
project(ShadowFrame)

# Find SDL3 package
find_package(SDL3 REQUIRED)

# Specify the source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add executable
add_executable(ShadowFrame ${SOURCES})

# Specify include directories
include_directories(src)

# Link SDL3 to the executable
target_link_libraries(ShadowFrame PRIVATE SDL3::SDL3)

# Specify C++ standard
set_target_properties(ShadowFrame PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# Optional: Set up include directories if needed
target_include_directories(ShadowFrame PRIVATE ${SDL3_INCLUDE_DIRS})