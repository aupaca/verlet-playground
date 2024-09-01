cmake_minimum_required(VERSION 3.14.5)
project(Verlet)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
add_subdirectory(./engine/andronaut)
include_directories(
    "./src/"
    "./src/render/"
    "./src/physic/"
    "./libs/glm/"
)
    
file(GLOB SRC_FILES 
    "./src/*.cpp" 
    "./src/render/*.cpp"
    "./src/physic/*.cpp"
)

add_library(verlet SHARED ${SRC_FILES})
target_link_libraries(verlet PRIVATE andronaut)
