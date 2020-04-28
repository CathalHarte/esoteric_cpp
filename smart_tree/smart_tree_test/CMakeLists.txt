cmake_minimum_required(VERSION 3.12.0)
project( smart_tree_test )

set(REPO_ROOT "../..")

set(CMAKE_CXX_STANDARD 17) # has to be at least 14 to use a make_unique (ptr)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Where to find other source files
add_subdirectory( .. smart_tree )

# The target
add_executable( smart_tree_test smart_tree_test.cpp )

target_include_directories( smart_tree_test PRIVATE   .. )

target_link_libraries( smart_tree_test smart_tree )
target_link_libraries( smart_tree_test libgtest.so libgtest_main.so libpthread.so )
