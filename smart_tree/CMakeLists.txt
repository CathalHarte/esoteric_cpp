cmake_minimum_required(VERSION 3.12.0)
project( smart_tree )

set(CMAKE_CXX_STANDARD 17) # should really be putting this elsewhere
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# The target
add_library( smart_tree smart_tree.cpp )
