cmake_minimum_required(VERSION 3.10)
project(WET2)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-std=c++11 -DNDEBUG -Wall")

add_executable(WET2 main.cpp Union.h Schedule.h Course.h Room.h Lecture.h methodTreeIntoArray.h library2.cpp library2.h Tuple.h)