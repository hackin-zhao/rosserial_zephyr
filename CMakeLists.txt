# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(external_lib)

target_sources(app PRIVATE src/main.c)

# The external static library that we are linking with does not know
# how to build for this platform so we export all the flags used in
# this zephyr build to the external build system.
#
# Other external build systems may be self-contained enough that they
# do not need any build information from zephyr. Or they may be
# incompatible with certain zephyr options and need them to be
# filtered out.
zephyr_get_include_directories_for_lang_as_string(       C includes)
zephyr_get_system_include_directories_for_lang_as_string(C system_includes)
zephyr_get_compile_definitions_for_lang_as_string(       C definitions)
zephyr_get_compile_options_for_lang_as_string(           C options)

set(external_project_cflags
  "${includes} ${definitions} ${options} ${system_includes}"
  )

include(ExternalProject)

# Add an external project to be able download and build the third
# party library. In this case downloading is not necessary as it has
# been committed to the repository.
set(ros_lib_src_dir   ${CMAKE_CURRENT_SOURCE_DIR}/ros_lib)
set(ros_lib_build_dir ${CMAKE_CURRENT_BINARY_DIR}/ros_lib)

set(ROS_LIB_LIB_DIR     ${ros_lib_build_dir})
set(ROS_LIB_INCLUDE_DIR ${ros_lib_src_dir})

ExternalProject_Add(
  ros_lib                 # Name for custom target
  PREFIX     ${ros_lib_build_dir} # Root dir for entire project
  SOURCE_DIR ${ros_lib_src_dir}
  BINARY_DIR ${ros_lib_build_dir} # This particular build system is invoked from the root
  CONFIGURE_COMMAND ${CMAKE_COMMAND} ${ros_lib_src_dir}
  INSTALL_COMMAND ""
  BUILD_BYPRODUCTS ${ROS_LIB_LIB_DIR}/libros_lib.a
  )

# Create a wrapper CMake library that our app can link with
add_library(zephyr_ros_lib STATIC IMPORTED GLOBAL)
add_dependencies(
  zephyr_ros_lib
  ros_lib
  )
set_target_properties(zephyr_ros_lib PROPERTIES IMPORTED_LOCATION             ${ROS_LIB_LIB_DIR}/libros_lib.a)
set_target_properties(zephyr_ros_lib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ROS_LIB_INCLUDE_DIR})

target_link_libraries(app PUBLIC zephyr_ros_lib)
