cmake_minimum_required(VERSION 3.13)
project(AnotherHourOfCode)
add_executable(AnotherHourOfCode main.cpp)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(AnotherHourOfCode -lOpenGL32 -lfreeGLUT)
target_link_libraries(AnotherHourOfCode -lglfw3dll -lopengl32)


