cmake_minimum_required(VERSION 3.0.0)
project(ChestnutStatemachine VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

# LIBRARY
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


# EXAMPLES

## Get dependencies
find_package(Threads)

add_executable(DoorStatemachineExample examples/door_statemachine.cpp)
target_link_libraries(DoorStatemachineExample PRIVATE ${PROJECT_NAME} Threads::Threads)

add_executable(LumberjackStatemachineExample examples/lumberjack_statemachine.cpp)
target_link_libraries(LumberjackStatemachineExample PRIVATE ${PROJECT_NAME} Threads::Threads)

add_executable(AeltoEventManagerExample examples/aelto_event_manager.cpp)
target_link_libraries(AeltoEventManagerExample PRIVATE ${PROJECT_NAME})

