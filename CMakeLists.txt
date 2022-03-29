cmake_minimum_required(VERSION 3.19)
project(ImageProcessOpencv)

set(CMAKE_CXX_STANDARD 14)

#  设置库、可执行文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#  添加包含路径
include_directories(${PROJECT_SOURCE_DIR}/include)
# Eigen
include_directories("/usr/include/eigen3")

# Ceres
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

#  添加opencv支持
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_library(ColorReduction src/ColorReduction.cpp)
target_link_libraries(ColorReduction ${OpenCV_LIBS})

add_library(Dithering src/Dithering.cpp)
target_link_libraries(Dithering ${OpenCV_LIBS})

add_library(Undistort src/Undistort.cpp)
target_link_libraries(Undistort ${OpenCV_LIBS} ${CERES_LIBRARIES})

add_executable(main_hw1 src/main_hw1.cpp)
target_link_libraries(main_hw1  ColorReduction
                                Dithering
                                ${OpenCV_LIBS})

add_executable(main_hw2 src/main_hw2.cpp)
target_link_libraries(main_hw2 Undistort
                                ${OpenCV_LIBS})

add_executable(main_hw4 src/main_hw4.cpp)
target_link_libraries(main_hw4 Hough
                                ${OpenCV_LIBS})

#   图像平滑
add_library(ImageSmoothing src/ImageSmoothing.cpp)
target_link_libraries(ImageSmoothing ${OpenCV_LIBS})

#   Hough 变换
add_library(Hough src/Hough.cpp)
target_link_libraries(Hough ${OpenCV_LIBS})


