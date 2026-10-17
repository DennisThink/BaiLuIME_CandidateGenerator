project(test3)
include_directories(../../../../thirdparty/doctest/doctest/)
SET(SOURCE_FILE ./main.cpp)
add_executable(test3 ${SOURCE_FILE})