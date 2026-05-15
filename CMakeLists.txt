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

FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_TAG v2.6.2)

FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
  GIT_TAG v0.45.0)

FetchContent_MakeAvailable(nlohmann_json cli11 httplib)

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

add_executable(${PROJECT_NAME} ${src_files})
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE CLI11::CLI11
  PRIVATE httplib::httplib)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
