cmake_minimum_required(VERSION 3.24)

project(neural_networks)

add_executable(${CMAKE_PROJECT_NAME}
	src/Matrix.h
	src/Matrix.cpp
	
	src/main.cpp
)