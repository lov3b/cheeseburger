cmake_minimum_required(VERSION 3.22)
project(cheese VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# default to Release
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified: defaulting to Release")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

include(CheckIncludeFileCXX)
check_include_file_cxx("format" HAS_STD_FORMAT)
if (NOT HAS_STD_FORMAT)
    message(STATUS "Using libfmt as a fallback for <format>")
    find_package(fmt REQUIRED)
    add_compile_definitions(USE_LIBFMT)
endif ()

find_package(Boost COMPONENTS program_options REQUIRED)

configure_file(
        "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
        "${PROJECT_BINARY_DIR}/version.hpp"
        @ONLY
)
include_directories("${PROJECT_BINARY_DIR}")

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(cheese ${SOURCES}
        src/format.hpp)

target_link_libraries(cheese LINK_PUBLIC ${Boost_LIBRARIES})

if (NOT HAS_STD_FORMAT)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
endif ()

# Warnings
if (MSVC)
    target_compile_options(cheese PRIVATE /W4 /WX)
else ()
    target_compile_options(cheese PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

install(TARGETS cheese
        RUNTIME DESTINATION bin
)
