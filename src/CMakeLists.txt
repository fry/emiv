# Gruppe 8
# Serie 2
# Alexander Koch, 773036
# Eike Siewertsen, 847522

#minimum version of cmake required to use this script
cmake_minimum_required(VERSION 2.4)
cmake_policy(SET CMP0003 NEW)

#set project name
PROJECT(EMultInf)

#set project version number
SET(EMultInf "0.1")

#set output directories. if not set, each binary will be placed where its source is
SET(LIBRARY_OUTPUT_PATH "${EMultInf_BINARY_DIR}/lib" CACHE PATH "This is where the libraries go.")
SET(EXECUTABLE_OUTPUT_PATH "${EMultInf_BINARY_DIR}/bin" CACHE PATH "This is where the binaries go.")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EMultInf_BINARY_DIR}/bin" CACHE PATH "This is where runtime DLLs go.")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -g")

# build shared or static libraries (never can build both) ?
OPTION(BUILD_SHARED_LIBS "Build EMultInf with shared libraries." ON)

## base include directory 
INCLUDE_DIRECTORIES(${EMultInf_SOURCE_DIR})

OPTION(USE_GTK "Use GTK+ instead of GLUT" OFF)

IF(USE_GTK)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGRAPHICS2D_USEGTK")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${EMultInf_SOURCE_DIR})
ENDIF(USE_GTK)

add_subdirectory(Graphics2DBase)
add_subdirectory(Graphics2D)

add_executable(testGraphics2D testGraphics2D.cpp)
target_link_libraries(testGraphics2D Graphics2D)

add_executable(filter filter.cpp)
target_link_libraries(filter Graphics2D)
