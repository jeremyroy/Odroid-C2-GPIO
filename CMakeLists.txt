cmake_minimum_required (VERSION 2.6)
project (Odroid-C2-GPIO CXX)

add_definitions(-std=c++11)

add_executable(gpio_class_test gpioc2_test.cpp)