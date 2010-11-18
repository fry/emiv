# Gruppe 8
# Serie 2
# Alexander Koch, 773036
# Eike Siewertsen, 847522
cmake_minimum_required(VERSION 2.4)
project(emiv2)
SET(LIBRARY_OUTPUT_PATH "${emiv2_BINARY_DIR}/lib")
SET(EXECUTABLE_OUTPUT_PATH "${emiv2_BINARY_DIR}/bin")
set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(.)
add_subdirectory(Graphics2D)
add_subdirectory(Graphics2DBase)

ADD_EXECUTABLE(testGraphics2D testGraphics2D.cpp)
target_link_libraries(testGraphics2D Graphics2D)
