cmake_minimum_required(VERSION 3.22)
project(SpriteRigger)

# Find SDL3 package
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

# Collect all source files (.cpp files)
file(GLOB_RECURSE SOURCES_FILES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/lib/*.cpp
)

# Include all source files (.cpp files)
add_executable(SpriteRigger ${SOURCES_FILES})

# Specify include directories. (header files)
target_include_directories(SpriteRigger PRIVATE 
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/include/imgui
  ${CMAKE_SOURCE_DIR}/include/nfd
)

# Link libraries
target_link_libraries(SpriteRigger PRIVATE
  SDL3::SDL3
  SDL3_image::SDL3_image

  # Libs for nfd
  gtk-3
  gobject-2.0
  glib-2.0
)

# Collect all bin files (.o files)
file(GLOB_RECURSE BIN_FILES ${CMAKE_SOURCE_DIR}/bin/*.o)
target_sources(SpriteRigger PRIVATE ${BIN_FILES})

# Specify C++ standard
set_target_properties(SpriteRigger PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)