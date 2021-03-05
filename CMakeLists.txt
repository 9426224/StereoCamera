cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

project(StereoCamera)

set(eSPDI_INCLUDE_DIR /home/ubuntu/StereoCamera/include/eSPDI/linux)
set(eSPDI_LIB_DIR /home/ubuntu/StereoCamera/lib/eSPDI/linux/aarch64)

find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP Lib is Found!")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
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
    -lJetsonGPIO
    ${eSPDI_LIB_DIR}/libeSPDI.so
    ${OpenCV_LIBS}
)


