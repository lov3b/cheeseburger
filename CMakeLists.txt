cmake_minimum_required(VERSION 3.30)
project(cheese VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# default to Release
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified: defaulting to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()


find_package(Boost COMPONENTS program_options REQUIRED)

configure_file(
        "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
        "${PROJECT_BINARY_DIR}/version.hpp"
        @ONLY
)
include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(cheese ${SOURCES})

target_link_libraries(cheese LINK_PUBLIC ${Boost_LIBRARIES})

# Warnings
if (MSVC)
    target_compile_options(cheese PRIVATE /W4 /WX)
else ()
    target_compile_options(cheese PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()