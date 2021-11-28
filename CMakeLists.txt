cmake_minimum_required(VERSION 3.0.0)
project(ChestnutStatemachine VERSION 2.0.0)
set(CMAKE_CXX_STANDARD 17)


add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(${PROJECT_NAME}_Example examples/door_statemachine.cpp)
find_package(Threads)
target_link_libraries(${PROJECT_NAME}_Example PRIVATE ${PROJECT_NAME} Threads::Threads)
