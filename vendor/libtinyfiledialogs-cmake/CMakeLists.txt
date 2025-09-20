set(SRC
  ../libtinyfiledialogs/tinyfiledialogs.c
)

add_library(libtinyfiledialogs STATIC ${SRC})

target_include_directories(libtinyfiledialogs PUBLIC
  ../libtinyfiledialogs
)