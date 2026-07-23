cmake_minimum_required(VERSION 4.2)
project(log4lib VERSION 1.0.0)

set(APP_TEST "${PROJECT_NAME}App")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(LOG4LIB_ENABLE_THREAD_SANITIZER "Build log4lib tests with ThreadSanitizer" ON)

include("./cmake/generatedoc.cmake")

#prevent having GTest in the deb package
set(INSTALL_GTEST OFF CACHE INTERNAL "")
set(INSTALL_GMOCK OFF CACHE INTERNAL "")
include(FetchContent)
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(gtest)

include_directories(./include)

set(SRC
        src/log4libbase.cpp
)

set(SRC_APP
        ${SRC}
        libtest/worker_in_lib.cpp
        libtest/worker_in_lib.hpp
)

set(TEST
        ${SRC}
        tests/test_log4lib_base.cpp
        tests/test_log4lib.cpp
        tests/test_log4lib_thread_safety.cpp
)

add_executable(${APP_TEST} ${SRC_APP} main.cpp)

enable_testing()
include(GoogleTest)
add_executable(${PROJECT_NAME}_test ${TEST})
gtest_discover_tests(${PROJECT_NAME}_test)
target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest_main GTest::gmock)

if (LOG4LIB_ENABLE_THREAD_SANITIZER)
        message(STATUS "Thread sanitizer enabled")
        target_compile_options(${PROJECT_NAME}_test PRIVATE -fsanitize=thread -g -O1)
        target_link_options(${PROJECT_NAME}_test PRIVATE -fsanitize=thread)
endif ()

add_library(${PROJECT_NAME} STATIC
        include/log4lib.hpp
        include/log4libbase.hpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
include("./cmake/generate_deb.cmake")
