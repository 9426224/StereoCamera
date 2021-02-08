cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_CXXSTANDARD 17)
set(CMALE_CXX_COMPILER "/usr/bin/g++")

add_compile_options(-std=c++17 -pthread)

project(StereoCamera)

set(eSPDI_INCLUDE_DIR /home/ubuntu/StereoCamera/include/eSPDI/linux)
set(eSPDI_LIB_DIR /home/ubuntu/StereoCamera/lib/eSPDI/linux/aarch64)

#find_package(OpenCV REQUIRED)
#if(OpenCV_FOUND)
#	message(STATUS "OpenCV Lib is Found!")
#endif()

find_package(ncnn)
if(ncnn_FOUND)
	message(STATUS "NCNN Lib is Found!")
endif()

set(OpenCV_INCLUDE_DIRS "/usr/local/include/opencv4")
set(OpenCV_LIBS "/usr/local/lib/")

include_directories(
	 ${eSPDI_INCLUDE_DIR}
	 ${OpenCV_INCLUDE_DIRS}
     ${ncnn_INCLUDE_DIRS}
)


aux_source_directory(./Stereo DIR_SRC)
add_executable(stereo
    ${DIR_SRC}
)

target_link_libraries(
    stereo
    ncnn
    ${eSPDI_LIB_DIR}/libeSPDI.so
    ${OpenCV_LIBS}
)


