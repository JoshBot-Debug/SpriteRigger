cmake_minimum_required(VERSION 3.22)
project(SpriteRigger)

# Find SDL3 package
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

# Collect all source files (.cpp files)
file(GLOB_RECURSE SOURCES_FILES
  ${CMAKE_SOURCE_DIR}/src/*.cpp

  # Application folder
  ${CMAKE_SOURCE_DIR}/Application/src/*.cpp
  ${CMAKE_SOURCE_DIR}/Application/lib/*.cpp
)

# Include all source files (.cpp files)
add_executable(SpriteRigger ${SOURCES_FILES})

# Specify include directories. (header files)
target_include_directories(SpriteRigger PRIVATE 
  ${CMAKE_SOURCE_DIR}/src

  # Application folder
  ${CMAKE_SOURCE_DIR}/Application/src
  ${CMAKE_SOURCE_DIR}/Application/include/imgui
  ${CMAKE_SOURCE_DIR}/Application/include/nfd
)

# Avoid shared libs, use static linking instead
# Collect all bin files (.o files)
# file(GLOB_RECURSE SHARED_LIB ${CMAKE_SOURCE_DIR}/Application/bin/*.o)
# target_sources(SpriteRigger PRIVATE ${SHARED_LIB})

# Collect all bin files (.a files)
file(GLOB_RECURSE STATIC_LIB ${CMAKE_SOURCE_DIR}/Application/bin/*.a)

# Link libraries
target_link_libraries(SpriteRigger PRIVATE
  SDL3
  SDL3_image
  OpenGL
  GLEW
  GLU
  assimp
  ${STATIC_LIB}

  # Libs for nfd
  gdk-3
  gtk-3
  gobject-2.0
  glib-2.0
)

# Specify C++ standard
set_target_properties(SpriteRigger PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

# Enable the best optimization settings
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -funroll-loops -fomit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-flto")

# Disable debug flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0")

# Strip symbols for production builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

# Enable fast math
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffast-math")
