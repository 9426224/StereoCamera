cmake_minimum_required(VERSION 2.8)

set(CMALE_CXX_COMPILER "/usr/local/bin/g++")

project(StereoCamera)

set(CMAKE_CXXSTANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${CUDA_TOOLKIT_ROOT_DIR}/lib")

set(PROJECT_SOURCE_DIR /home/ubuntu/StereoCamera)

set(eSPDI_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/eSPDI/linux)

set(eSPDI_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/eSPDI/linux/aarch64)

aux_source_directory(./Stereo DIR_SRC)

add_executable(stereo
    ${DIR_SRC}
)

find_package(CUDA REQUIRED)
if(CUDA_FOUND)
	message(STATUS "CUDA Lib is Found!")
endif()

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
	message(STATUS "OpenCV Lib is Found!")
endif()

find_package(ncnn)
if(ncnn_FOUND)
	message(STATUS "NCNN Lib is Found!")
endif()

include_directories(
	 ${eSPDI_INCLUDE_DIR}
	 ${CUDA_INCLUDE_DIRS}
	 ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
    stereo
    
    ${eSPDI_LIB_DIR}/libeSPDI.so
    ${OpenCV_LIBS}
)
