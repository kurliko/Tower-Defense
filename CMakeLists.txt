cmake_minimum_required(VERSION 3.16)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(TowerDefense ${SOURCES})
target_link_libraries(TowerDefense sfml-graphics sfml-window sfml-system)
