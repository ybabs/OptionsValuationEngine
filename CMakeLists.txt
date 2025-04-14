cmake_minimum_required(VERSION 3.15)
project(ValuationEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake OPTIONAL)
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB_RECURSE SOURCES  "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(ValuationEngine ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/inc)


target_link_libraries(ValuationEngine ${CONAN_LIBS})