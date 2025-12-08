cmake_minimum_required(VERSION 3.8)

project(nvtracker LANGUAGES CXX CUDA) 

find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O0 -g")

pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-video-1.0 gstreamer-base-1.0)

link_directories("/opt/nvidia/deepstream/deepstream-6.0/lib" "/usr/local/cuda/lib64" "/usr/lib/x86_64-linux-gnu")
include_directories(${CMAKE_SOURCE_DIR}/include ${GSTREAMER_INCLUDE_DIRS} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

add_library(tracker SHARED ./src/tracker.cpp)
target_include_directories(tracker PUBLIC
    $<BUILD_INTERFACE:/opt/nvidia/deepstream/deepstream-6.0/sources/includes>
    $<INSTALL_INTERFACE:include/>
)
 
target_link_libraries(tracker PRIVATE 
    ${GLIB_LIBRARIES} 
    ${GSTREAMER_LIBRARIES} 
    ${CMAKE_CUDA_TOOLKIT_LIBRARIES} 
    nvdsgst_meta 
    nvds_meta
    cudart
)

add_executable(main ./src/main.cpp)
target_link_libraries(main PRIVATE tracker)

set_property(TARGET main PROPERTY CXX_STANDARD 17)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)