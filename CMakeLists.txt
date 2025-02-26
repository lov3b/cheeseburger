cmake_minimum_required(VERSION 3.30)
project(cheese VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS program_options REQUIRED)

configure_file(
        "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
        "${PROJECT_BINARY_DIR}/version.hpp"
        @ONLY
)
include_directories("${PROJECT_BINARY_DIR}")

add_executable(cheese src/main.cpp
        src/args.cpp
        src/cheese.cpp
        src/terminal.cpp
        src/version.hpp.in
)

target_link_libraries(cheese LINK_PUBLIC ${Boost_LIBRARIES})
