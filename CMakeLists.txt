cmake_minimum_required(VERSION 3.0)

project(com_man)

add_custom_target(
  format 
  COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/com_man_host && format_source && cd ../com_man_common && format_source)

add_subdirectory(lib/channels/channels)
add_subdirectory(com_man_host)