cmake_minimum_required(VERSION 3.0.0)
project(ChestnutStatemachineExample VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads)
add_executable(ChestnutStatemachineExample examples/door_statemachine.cpp)
target_link_libraries(ChestnutStatemachineExample PRIVATE Threads::Threads)
