cmake_minimum_required(VERSION 3.24)

project(neural_networks)

add_executable(${CMAKE_PROJECT_NAME}
	src/main.cpp
	
	src/Matrix.h
	src/Matrix.cpp

	src/Network.h
	src/Network.cpp
	
	src/ActivationFunction.h
	src/ActivationFunction.cpp
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})