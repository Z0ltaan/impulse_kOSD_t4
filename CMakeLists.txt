cmake_minimum_required(VERSION 3.22)

project(
  media_finder
  VERSION 0.1.0
  LANGUAGES CXX)

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

add_executable(${PROJECT_NAME} ${src_files})
