cmake_minimum_required(VERSION 4.2)
project(log4lib VERSION 1.0.0)

set(APP_TEST "${PROJECT_NAME}App")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
)

add_executable(${APP_TEST} ${SRC_APP} main.cpp)

enable_testing()
include(GoogleTest)
add_executable(${PROJECT_NAME}_test ${TEST})
gtest_discover_tests(${PROJECT_NAME}_test)
target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest_main GTest::gmock)

add_library(${PROJECT_NAME} STATIC
        include/log4lib.hpp
        include/log4libbase.hpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
#include("./cmake/generate_deb.cmake")
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/man/man3
        DESTINATION share/man/man3
        COMPONENT documentation
)

set(CPACK_GENERATOR "DEB")

# Mandatory package metadata
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "free")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "maintainer@example.com")
set(CPACK_PACKAGE_DESCRIPTION "A brief description of my application.")
set(CPACK_DEBIAN_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # Automatically find system dependencies
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(CPACK_SOURCE_IGNORE_FILES
        libtest/
        tests/
)
include(CPack)
