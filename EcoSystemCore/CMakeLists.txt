cmake_minimum_required(VERSION 3.20)
project(EcoSystemCore VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем все исходные файлы
file(GLOB_RECURSE SOURCES 
    "Core/*.cpp"
    "Entities/*.cpp"
    "World/*.cpp"
    "Simulation/*.cpp"
)

file(GLOB_RECURSE HEADERS 
    "Core/*.h"
    "Entities/*.h"
    "World/*.h"
    "Simulation/*.h"
)

add_library(EcoSystemCore STATIC ${SOURCES} ${HEADERS})

target_include_directories(EcoSystemCore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/Entities
    ${CMAKE_CURRENT_SOURCE_DIR}/World
    ${CMAKE_CURRENT_SOURCE_DIR}/Simulation
)

# Пример использования
add_executable(EcoSystemDemo examples/main.cpp)
target_link_libraries(EcoSystemDemo EcoSystemCore)
target_link_libraries(EcoSystemDemo pthread)
