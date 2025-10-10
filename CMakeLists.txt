cmake_minimum_required(VERSION 3.22)
project(ssrd)

# === Options ===
option(ENABLE_STL_DEBUG "Enable STL debug mode." OFF)
option(ENABLE_THREAD_SANITIZER "Enable ThreadSanitizer (disables AddressSanitizer)" OFF)

# === Compiler Flags ===
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Add vendor libraries ===
add_subdirectory(vendor/imgui-cmake)
add_subdirectory(vendor/tinyfiledialogs-cmake)

# === Source Files ===
file(GLOB_RECURSE SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE GLAD_SRC ${CMAKE_SOURCE_DIR}/vendor/glad/src/*.c)

# === Find Libraries ===
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
message(STATUS "OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")

# === Create library from project ===
add_library(sprigg ${SRC} ${GLAD_SRC})

# === Include Directories ===
target_include_directories(sprigg PRIVATE 
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/vendor/glad/include
  ${CMAKE_SOURCE_DIR}/vendor/imgui
  ${CMAKE_SOURCE_DIR}/vendor/imgui/backends
  ${CMAKE_SOURCE_DIR}/vendor/tinyfiledialogs
)

# === Link Libraries ===
target_link_libraries(sprigg PRIVATE
  ${OPENGL_LIBRARIES}
  imgui
  tinyfiledialogs
)

# === Create the executable ===
add_executable(SpriteRigger src/main.cpp)

# === Include Directories ===
target_include_directories(SpriteRigger PRIVATE 
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/vendor/glad/include
  ${CMAKE_SOURCE_DIR}/vendor/imgui
  ${CMAKE_SOURCE_DIR}/vendor/imgui/backends
  ${CMAKE_SOURCE_DIR}/vendor/tinyfiledialogs
)

# === Link the Project Library to the executable ===
target_link_libraries(SpriteRigger PRIVATE sprigg)

# === Google Test Setup ===
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.17.0   # or latest stable
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# === Tests executable ===
file(GLOB_RECURSE TEST_SRC ${CMAKE_SOURCE_DIR}/tests/*.cpp)

add_executable(tests ${TEST_SRC})
target_link_libraries(tests PRIVATE gtest_main sprigg)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(GoogleTest)
gtest_discover_tests(tests)

# === Optional: ccache (speed up rebuilds) ===
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
  message(STATUS "ccache is ENABLED.")
endif()

# === Debug Configuration ===
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG -D_GLIBCXX_DEBUG)

  if(ENABLE_STL_DEBUG)
    target_compile_options(SpriteRigger PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wshadow
      -Wconversion
      -Wsign-conversion
      -Wnull-dereference
      -Wdouble-promotion
      -Wformat=2
      -Wunused
    )

    message(STATUS "STL debug mode is ENABLED.")

    if(ENABLE_THREAD_SANITIZER)
      set(SANITIZE_FLAGS "-fsanitize=thread")
      message(STATUS "Using ThreadSanitizer (no ASan/UBSan).")
    else()
      set(SANITIZE_FLAGS "-fsanitize=undefined,address -fno-omit-frame-pointer")
      message(STATUS "Using AddressSanitizer + UndefinedBehaviorSanitizer.")
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG "${SANITIZE_FLAGS}")
    set(CMAKE_LINKER_FLAGS_DEBUG "${SANITIZE_FLAGS}")
  else()
  endif()

  message(STATUS "Debug is ENABLED.")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

# === Release Configuration ===
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto=auto -funroll-loops -fomit-frame-pointer -g0 -s -ffast-math")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto=auto")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-flto=auto")