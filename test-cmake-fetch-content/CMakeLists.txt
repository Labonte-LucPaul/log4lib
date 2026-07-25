cmake_minimum_required(VERSION 4.2)
project(log4libtestfetchcontent VERSION 0.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

message("Testing fetch content!!!")
include(FetchContent)
FetchContent_Declare(
        log4lib
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
)

FetchContent_MakeAvailable(log4lib)

add_executable(${PROJECT_NAME} test-fetch-content.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC log4lib::log4lib)