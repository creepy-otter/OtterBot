cmake_minimum_required(VERSION 3.14)
project(otterbot)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

message(STATUS "SOURCES: ${SOURCES}")

add_executable(${PROJECT_NAME} ${SOURCES})

# Uncomment and modify the lines below to link external libraries, if needed
# find_package(SomeExternalLib REQUIRED)
# target_link_libraries(${PROJECT_NAME} SomeExternalLib::SomeExternalLib)
