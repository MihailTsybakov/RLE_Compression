cmake_minimum_required(VERSION 3.1)

project(Task_4_Sem_3)

include_directories(${PROJECT_SOURCE_DIR})

set(TASK_4_SRC main.cpp funcs.cpp RLE_Compressor.cpp)
set(TASK_4_HDR funcs.h RLE_Compressor.h)

add_executable(${PROJECT_NAME} ${TASK_4_SRC} ${TASK_4_HDR})