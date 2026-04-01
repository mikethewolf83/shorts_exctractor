cmake_minimum_required(VERSION 3.15)
project(shorts_extractor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Find FFmpeg
find_package(PkgConfig QUIET)
pkg_check_modules(FFMPEG QUIET libavcodec libavformat libavutil)

# If pkg-config fails, look for ffmpeg executable
if(NOT FFMPEG_FOUND)
    find_program(FFMPEG_BIN ffmpeg)
    if(FFMPEG_BIN)
        set(FFMPEG_FOUND TRUE)
        message(STATUS "Found FFmpeg: ${FFMPEG_BIN}")
    endif()
endif()

# Main executable
add_executable(shorts_extractor src/main.cpp)
target_include_directories(shorts_extractor PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(WIN32)
    target_link_libraries(shorts_extractor PRIVATE)
endif()

# Install rules
install(TARGETS shorts_extractor DESTINATION bin)
install(FILES ffmpeg.exe DESTINATION bin OPTIONAL)

# Tests
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()

    # Download GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(test_time_parser tests/test_time_parser.cpp)
    target_link_libraries(test_time_parser PRIVATE GTest::gtest_main)
    target_include_directories(test_time_parser PRIVATE ${CMAKE_SOURCE_DIR}/include)

    add_test(NAME TimeParserTests COMMAND test_time_parser)
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
