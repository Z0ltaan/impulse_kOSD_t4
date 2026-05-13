cmake_minimum_required(VERSION 3.22)

project(
  media_finder
  VERSION 0.1.0
  LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.12.0)

FetchContent_MakeAvailable(nlohmann_json)

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

add_executable(${PROJECT_NAME} ${src_files})
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
