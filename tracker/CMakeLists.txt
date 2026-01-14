cmake_minimum_required(VERSION 3.28)

project(nvtracker)


IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -MMD -Wall -Wextra -Winit-self")


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
if(COMPILER_SUPPORTS_CXX20)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
  add_definitions(-DCOMPILEDWITHC20)
  message(STATUS "Using flag -std=c++20.")
else()
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++20 support. Please use a different C++ compiler.")
endif()


set(EIGEN_VERSION 3.3)
find_package(Eigen3 ${EIGEN_VERSION} QUIET)
find_package(OpenCV REQUIRED)
find_package(onnxruntime REQUIRED PATHS "/usr/local/lib")


if(NOT EIGEN3_FOUND)
  set(BUILD_TESTING OFF CACHE INTERNAL "")
  FetchContent_Declare(eigen
    GIT_REPOSITORY  https://gitlab.com/libeigen/eigen.git
    GIT_TAG         ${EIGEN_VERSION}
    GIT_SHALLOW     ON)
  FetchContent_MakeAvailable(eigen)
  unset(BUILD_TESTING CACHE)
endif()

add_library(${PROJECT_NAME} SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/src/BYTETracker.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/KalmanFilter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lapjv.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Object.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Rect.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/STrack.cpp
  )
target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen onnxruntime)


if(BUILD_TEST_PROGRAM)
  add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)
  target_link_libraries(test
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    onnxruntime
  )
endif()
