cmake_minimum_required(VERSION 2.8)
project( existencia_racao_1 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( existencia_racao_1 existencia_racao_1.cpp )
target_link_libraries( existencia_racao_1 ${OpenCV_LIBS} )