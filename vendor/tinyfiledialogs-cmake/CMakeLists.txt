set(SRC
  ../tinyfiledialogs/tinyfiledialogs.cpp
)

add_library(tinyfiledialogs STATIC ${SRC})

target_include_directories(tinyfiledialogs PUBLIC
  ../tinyfiledialogs
)