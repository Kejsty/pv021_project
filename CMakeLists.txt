cmake_minimum_required(VERSION 3.3)
project(pv021_project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic")

set(SOURCES NeuralNetwork.cpp Layer.h NeuralNetwork.h
        algorithms.cpp main.cpp HiddenLayer.cpp HiddenLayer.h OutputLayer.cpp OutputLayer.h InputLayer.cpp InputLayer.h HiddenLayerSnapshot.h OutputLayerSnapshot.h NeuralNetworkExceptions.h ImageMaker.cpp ImageMaker.h)
add_executable(neuralNetwork ${SOURCES} ImageMaker.cpp ImageMaker.h)

include_directories(pv021_project)
include_directories(SYSTEM libs/googletest/include)
add_subdirectory(libs/googletest)


file(GLOB TEST_SOURCES tests/*.cpp algorithms.h algorithms.cpp)
add_executable(pv021_tests ${TEST_SOURCES})
target_link_libraries(pv021_tests gtest gtest_main)
