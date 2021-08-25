cmake_minimum_required(VERSION 3.7.2)
project(ros_lib)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_library(ros_lib STATIC
  duration.cpp
  time.cpp
)