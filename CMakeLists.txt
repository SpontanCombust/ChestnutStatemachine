cmake_minimum_required(VERSION 3.0.0)
project(ChestnutStatemachine VERSION 2.2.0)
set(CMAKE_CXX_STANDARD 17)

# LIBRARY
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


# EXAMPLES
add_executable(DoorStatemachineExample examples/door_statemachine.cpp)
find_package(Threads)
target_link_libraries(DoorStatemachineExample PRIVATE ${PROJECT_NAME} Threads::Threads)

add_executable(AnimalStatemachineExample examples/animal_statemachine.cpp)
target_link_libraries(AnimalStatemachineExample PRIVATE ${PROJECT_NAME})

