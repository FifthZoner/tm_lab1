cmake_minimum_required(VERSION 3.31)
project(tm_lab1)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.x)
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 20)

add_executable(tm_lab1 main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-network Xrandr armadillo)