cmake_minimum_required(VERSION 3.22)
project(ShadowFrame)

# Find SDL3 package
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(Vulkan REQUIRED)

# Collect all source files (.cpp files)
file(GLOB_RECURSE SOURCES_FILES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/lib/*.cpp
)

# Include all source files (.cpp files)
add_executable(ShadowFrame ${SOURCES_FILES})

# Specify include directories. (header files)
target_include_directories(ShadowFrame PRIVATE 
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/include/imgui
)

# Link libraries
target_link_libraries(ShadowFrame PRIVATE
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  Vulkan::Vulkan
  pthread
  dl
)

# Specify C++ standard
set_target_properties(ShadowFrame PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)