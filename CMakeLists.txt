cmake_minimum_required(VERSION 3.14)

project(otterbot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-D_REENTRANT -D_DEBUG)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(include)

set(TWS_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/TwsApiCpp/TwsApiC++)
set(API_INCLUDE_DIR ${TWS_API_DIR}/Api)
set(IB_POSIX_CLIENT_SRC ${TWS_API_DIR}/../source/PosixClient)
set(IB_POSIX_CLIENT_INCLUDE_DIR ${IB_POSIX_CLIENT_SRC}/Shared)
set(IB_POSIX_CLIENT_SOURCE_DIR ${IB_POSIX_CLIENT_SRC}/src)

file(GLOB IB_POSIX_CLIENT_SOURCES ${IB_POSIX_CLIENT_SOURCE_DIR}/*)
file(GLOB IB_POSIX_CLIENT_INCLUDES ${IB_POSIX_CLIENT_INCLUDE_DIR}/*.h)
file(GLOB API_INCLUDES ${API_INCLUDE_DIR}/*.h)

include_directories(${API_INCLUDE_DIR} ${IB_POSIX_CLIENT_INCLUDE_DIR})

file(GLOB_RECURSE OTTER_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${OTTER_SOURCES})

add_subdirectory(TwsApiCpp)

# target_link_libraries(${PROJECT_NAME} tws_cpp_api Threads::Threads)
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# Uncomment and modify the lines below to link external libraries, if needed
# find_package(SomeExternalLib REQUIRED)
# target_link_libraries(${PROJECT_NAME} SomeExternalLib::SomeExternalLib)
