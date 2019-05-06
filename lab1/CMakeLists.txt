cmake_minimum_required(VERSION 3.13)
project(lab1)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab1 main.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(/usr/local/Cellar/opencv/4.0.1/include/opencv4)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

cmake_minimum_required(VERSION 3.13)
project(zad1)