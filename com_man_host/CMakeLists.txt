cmake_minimum_required(VERSION 3.0)

project(com_man_host)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SRC_FILES "src/*.c*")
file(GLOB HDR_FILES "include/*.h*")

set (COM_MAN_HOST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include PARENT_SCOPE)

include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../com_man_common/include)
include_directories(${CHANNELS_LIB_INCLUDE_DIR})

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} channels)

add_dependencies(${PROJECT_NAME} format)