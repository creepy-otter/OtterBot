cmake_minimum_required(VERSION 3.14)

project(otterbot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
  glog
  GIT_REPOSITORY https://github.com/google/glog.git
  GIT_TAG        v0.5.0
)
FetchContent_MakeAvailable(glog)

include_directories(include)

set(TWS_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodule/TwsApiCpp/TwsApiC++)
set(API_INCLUDE_DIR ${TWS_API_DIR}/Api)
set(IB_POSIX_CLIENT_SRC ${TWS_API_DIR}/../source/PosixClient)
set(IB_POSIX_CLIENT_INCLUDE_DIR ${IB_POSIX_CLIENT_SRC}/Shared)
set(IB_POSIX_CLIENT_SOURCE_DIR ${IB_POSIX_CLIENT_SRC}/src)

file(GLOB IB_POSIX_CLIENT_SOURCES ${IB_POSIX_CLIENT_SOURCE_DIR}/*)
file(GLOB IB_POSIX_CLIENT_INCLUDES ${IB_POSIX_CLIENT_INCLUDE_DIR}/*.h)
file(GLOB API_INCLUDES ${API_INCLUDE_DIR}/*.h)

include_directories(${API_INCLUDE_DIR} ${IB_POSIX_CLIENT_INCLUDE_DIR})

add_subdirectory(submodule/TwsApiCpp/TwsApiC++)

file(GLOB_RECURSE OTTER_SOURCES "src/*.cpp")

message(STATUS "OTTER_SOURCES: ${OTTER_SOURCES}")

add_executable(${PROJECT_NAME} tests/otterbot.cpp ${OTTER_SOURCES})
target_link_libraries(${PROJECT_NAME} tws_cpp_api Threads::Threads glog::glog)

add_executable(ibkrbot tests/ibkrbot.cpp ${OTTER_SOURCES})
target_link_libraries(ibkrbot tws_cpp_api Threads::Threads glog::glog)